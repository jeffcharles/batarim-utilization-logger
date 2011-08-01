// references and statements required by COM
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comutil.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")
// -----------------------------------------

#include <string>
#include <vector>

#include "cpu_usage.hpp"
#include "windows_com_setup_exception.hpp"
#include "windows_wmi_exception.hpp"

void initialize_com_interface()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if(FAILED(hr)) {
        throw com_setup_exception("Failed to initialize COM interface.", hr);
    }
}

void initialize_com_security()
{
    HRESULT hr = CoInitializeSecurity(
        NULL, // Security descriptor
        -1, // COM negotiates authentication service
        NULL, // Authentication services
        NULL, // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT, // Default authentication level for proxies
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default impersonation level for proxies
        NULL, // Authentication info
        EOAC_NONE, // Additional capabilities of the client or server
        NULL
    );
    if(FAILED(hr)) {
        throw com_setup_exception("Failed to initialize COM security.", hr);
    }
}

void create_connection_to_wmi_namespace(IWbemLocator*& pLoc)
{
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, NULL, 
        CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if(FAILED(hr)) {
        throw wmi_exception(
            "Failed to create connection to WMI namespace.", 
            hr
        );
    }
}

void get_wmi_proxy(IWbemLocator* pLoc, IWbemServices*& pSvc)
{
    HRESULT hr = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // local WMI network resource
        NULL, // user (null is currently logged in user)
        NULL, // password (null is currently logged in user)
        NULL, // locale (null is default)
        0, // return once connection is established
        NULL, // domain name of user to log in as (NULL is NTLM auth of user)
        NULL, // object required by one or more dynamic class providers
        &pSvc // pointer bound to IWbemServices object in specified namespace
    );
    if(FAILED(hr)) {
        throw wmi_exception("Could not get WMI proxy.", hr);
    }
}

void set_wmi_security(IWbemLocator* pLoc, IWbemServices* pSvc)
{
    HRESULT hr = CoSetProxyBlanket(
        pSvc, // proxy to be set
        RPC_C_AUTHN_WINNT, // authentication service to use (NTLMSSP)
        RPC_C_AUTHZ_NONE, // authorization service to use (none)
        NULL, // server principal name (NULL = do not reset principal name)
        RPC_C_AUTHN_LEVEL_CALL, // authenticate only at beginning of RPCs
        RPC_C_IMP_LEVEL_IMPERSONATE, // impersonates client's security context
        NULL, // establishes id of client (NULL = use currenty proxy identity)
        EOAC_NONE // capabilities of the proxy
    );
    if(FAILED(hr)) {
        throw wmi_exception("Could not set WMI proxy security.", hr);
    }
}

void clean_up_wmi(IWbemLocator*& loc, IWbemServices*& svc)
{
    if(svc != NULL) {
        svc->Release();
    }
    if(loc != NULL) {
        loc->Release();
    }
    CoUninitialize();
}

void exec_query(IWbemServices *svc, IEnumWbemClassObject*& result_enum)
{
    HRESULT hr = svc->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t(
            "SELECT Name, PercentProcessorTime "
            "FROM Win32_PerfFormattedData_PerfOS_Processor"
        ),
        WBEM_FLAG_FORWARD_ONLY, // return forward-only enumerator
        NULL, // pointer to IWbemContext (NULL = no context required)
        &result_enum
    );
    if(FAILED(hr)) {
        throw wmi_exception(
            "Could not query WMI for processor utilization percentage.", 
            hr
        );
    }
}

void get_wstring_property_from_record(
    const string& property_name, 
    IWbemClassObject*& record, 
    wstring& out
)
{
    VARIANT prop;
    HRESULT hr = record->Get(
        _bstr_t(property_name.c_str()),
        0, // reserved
        &prop, // output
        NULL, // type of property (optional)
        NULL // information about origin of property (optional)
    );
    if(FAILED(hr)) {
        throw wmi_exception(
            "Error extracting processor " + property_name + " while "
            "retrieving processor utilization percentage", 
            hr
        );
    }
    out = prop.bstrVal;
    VariantClear(&prop);
}

void get_uint64_property_from_record(
    const string& property_name, 
    IWbemClassObject*& record, 
    long& out
)
{
    VARIANT prop;
    HRESULT hr = record->Get(
        _bstr_t(property_name.c_str()),
        0, // reserved
        &prop, // output
        NULL, // type of property (optional)
        NULL // information about origin of property (optional)
    );
    if(FAILED(hr)) {
        throw wmi_exception(
            "Error extracting processor " + property_name + " while "
            "retrieving processor utilization percentage", 
            hr
        );
    }
    VarI4FromStr(prop.bstrVal, NULL, 0, &out);
    VariantClear(&prop);
}

void query_wmi(IWbemServices *svc, vector<pair<wstring, int> >& usage_percentages)
{
    IEnumWbemClassObject* result_enum;
    exec_query(svc, result_enum);
    while(result_enum) {
        IWbemClassObject* record;
        ULONG num_objects_returned;
        HRESULT hr = result_enum->Next(
            WBEM_INFINITE, // max amount of time to wait for result
            1, // number of requested objects
            &record,
            &num_objects_returned
        );
        if(num_objects_returned == 0) {
            break;
        }
        if(FAILED(hr)) {
            throw wmi_exception(
                "Could not advance enumerator for processor utilization "
                "percentage.",
                hr
            );
        }
        
        wstring name;
        get_wstring_property_from_record("Name", record, name);
        long l_usage_percentage;
        get_uint64_property_from_record("PercentProcessorTime", record, l_usage_percentage);
        
        record->Release();
        
        pair<wstring, int> processor_info (name, (int)l_usage_percentage);
        usage_percentages.push_back(processor_info);
    }
}

void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
    IWbemLocator* loc = NULL;
    IWbemServices* svc = NULL;
    try {
        initialize_com_interface();
        initialize_com_security();
        create_connection_to_wmi_namespace(loc);
        get_wmi_proxy(loc, svc);
        set_wmi_security(loc, svc);
        query_wmi(svc, usage_percentages);
        clean_up_wmi(loc, svc);
    } catch(com_setup_exception& e) {
        cerr << e.what() << endl;
        clog << e.what() << endl;
    } catch(wmi_exception& e) {
        cerr << e.what() << endl;
        clog << e.what() << endl;
        clean_up_wmi(loc, svc);
    }
}
