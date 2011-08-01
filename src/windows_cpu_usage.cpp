// references and statements required by COM
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comutil.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")
// -----------------------------------------

#include "cpu_usage.hpp"
#include "windows_com_setup_exception.hpp"
#include "windows_wmi_exception.hpp"

void initialize_com_interface()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if(FAILED(hr)) {
        cerr << "Failed to initialize COM interface. Error code = 0x"
            << hex << hr << endl;
        throw com_setup_exception();
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
        cerr << "Failed to initialize COM security. Error code = 0x"
            << hex << hr << endl;
        throw com_setup_exception();
    }
}

void create_connection_to_wmi_namespace(IWbemLocator*& pLoc)
{
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, NULL, 
        CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if(FAILED(hr)) {
        cerr << "Failed to create connection to WMI namespace. Error code = 0x"
            << hex << hr << endl;
        CoUninitialize();
        throw wmi_exception();
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
        cerr << "Could not get WMI proxy. Error code = 0x"
            << hex << hr << endl;
        pLoc->Release();
        CoUninitialize();
        throw wmi_exception();
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
        cerr << "Could not set WMI proxy security. Error code = 0x"
            << hex << hr << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        throw wmi_exception();
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

void query_wmi(IWbemServices *svc, int* usage_percentage)
{
    IEnumWbemClassObject* result_enum;
    HRESULT hr = svc->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t(
            "SELECT PercentProcessorTime "
            "FROM Win32_PerfFormattedData_PerfOS_Processor"
        ),
        WBEM_FLAG_FORWARD_ONLY, // return forward-only enumerator
        NULL, // pointer to IWbemContext (NULL = no context required)
        &result_enum
    );
    switch(hr) {
        case WBEM_E_ACCESS_DENIED:
            cerr << "The current user does not have permission to view the " 
                << "processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_FAILED:
            cerr << "Unspecified error when trying to view the processor "
                << "utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_INVALID_PARAMETER:
            cerr << "Invalid parameter specified when trying to view the "
                << "processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_INVALID_QUERY:
            cerr << "The query was not syntactically correct when trying to "
                << "view the processor utlization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_INVALID_QUERY_TYPE:
            cerr << "The requested query language was not supported when "
                << "trying to view the processor utilization percentage"
                << endl;
            throw wmi_exception();
        case WBEM_E_OUT_OF_MEMORY:
            cerr << "There was not enough memory to view the processor "
                << "utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_SHUTTING_DOWN:
            cerr << "Windows Management service was stopped and restarted "
                << "when trying to view the processor utilization percentage"
                << endl;
            throw wmi_exception();
        case WBEM_E_TRANSPORT_FAILURE:
            cerr << "Remote Procedure Call failed when trying to view the "
                << "processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_NOT_FOUND:
            cerr << "The query specified a class that does not exist when "
                << "trying to view the processor utilization percentage"
                << endl;
            throw wmi_exception();
        case WBEM_S_NO_ERROR:
            break;
        default:
            cerr << "Unkown error occurred when trying to view the processor "
                << "utilization percentage" << endl;
            throw wmi_exception();
    }

    IWbemClassObject* record;
    ULONG num_objects_returned;
    HRESULT enum_result = result_enum->Next(
        WBEM_INFINITE, // max amount of time to wait for result
        1, // number of requested objects
        &record,
        &num_objects_returned
    );
    switch(enum_result) {
        case WBEM_E_INVALID_PARAMETER:
            cerr << "One or more invalid parameters were supplied in the call "
                << "to the processor utilization query enumerator" << endl;
            throw wmi_exception();
        case WBEM_E_OUT_OF_MEMORY:
            cerr << "There was not enough memory to complete the enumeration "
                << "when trying to view the processor utilization percentage"
                << endl;
            throw wmi_exception();
        case WBEM_E_UNEXPECTED:
            cerr << "An object in the enumeration for viewing the processor "
                << "utilization percentage has been deleted" << endl;
            throw wmi_exception();
        case WBEM_E_TRANSPORT_FAILURE:
            cerr << "A Remote Procedure Call to advance the enumberator for "
                << "viewing the processor utilization percentage has failed"
                << endl;
            throw wmi_exception();
        case WBEM_S_NO_ERROR:
            break;
        case WBEM_S_FALSE:
            cerr << "Unable to obtain processor utilization percentage from "
                << "enumerator" << endl;
            throw wmi_exception();
        case WBEM_S_TIMEDOUT:
            cerr << "Advancing enumerator timed out when trying to view "
                << "processor utilization percentage" << endl;
            throw wmi_exception();
        default:
            cerr << "Unkown error occurred when trying to advance enumerator "
                << "when trying to view processor utilization percentage"
                << endl;
            throw wmi_exception();
    }
    VARIANT percent_processor_time;
    HRESULT record_result = record->Get(
        L"PercentProcessorTime", // property name
        0, // reserved
        &percent_processor_time, // output
        NULL, // type of property (optional)
        NULL // information about origin of property (optional)
    );
    switch(record_result) {
        case WBEM_E_FAILED:
            cerr << "There was a general failure when trying to extract "
                << "the processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_INVALID_PARAMETER:
            cerr << "One or more parameters was not valid when trying to "
                << "extract the processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_S_NO_ERROR:
            break;
        case WBEM_E_NOT_FOUND:
            cerr << "The specified property was not found when trying to "
                << "extract the processor utilization percentage" << endl;
            throw wmi_exception();
        case WBEM_E_OUT_OF_MEMORY:
            cerr << "There was not enough memory to complete the operation to "
                << "extract the processor utilization percentage" << endl;
            throw wmi_exception();
        default:
            cerr << "An unknown error occurred when trying to extract the "
                << "processor utilization percentage" << endl;
            throw wmi_exception();
    }
    long l_usage_percentage;
    VarI4FromStr(percent_processor_time.bstrVal, NULL, 0, &l_usage_percentage);
    VariantClear(&percent_processor_time);
    record->Release();
    *usage_percentage = (int)l_usage_percentage;
}

void get_cpu_usage(int* usage_percentage)
{
    IWbemLocator* loc = NULL;
    IWbemServices* svc = NULL;
    try {
        initialize_com_interface();
        initialize_com_security();
        create_connection_to_wmi_namespace(loc);
        get_wmi_proxy(loc, svc);
        set_wmi_security(loc, svc);
        query_wmi(svc, usage_percentage);
        clean_up_wmi(loc, svc);
    } catch(com_setup_exception& e) {
        // TODO: log exception details
    } catch(wmi_exception& e) {
        // TODO: log exception details
        clean_up_wmi(loc, svc);
    }
}
