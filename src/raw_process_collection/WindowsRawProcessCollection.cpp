#include <memory>
#include <vector>

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "../utilities/utilities.hpp"

#include "WindowsRawProcessCollection.hpp"

using std::unique_ptr;
using std::string;
using std::vector;
using std::wstring;

std::unique_ptr<std::vector<unsigned int>>
WindowsRawProcessCollection::get_pids_() const
{
    const DWORD pid_array_length = 0x10000;
    DWORD pids[pid_array_length];
    DWORD bytes_returned;
    if(!EnumProcesses(pids, pid_array_length, &bytes_returned)) {
        return false;
    }
    const DWORD num_pids = bytes_returned / sizeof(DWORD);

    unique_ptr<vector<unsigned int>> ret(new vector<unsigned int>);
    ret->resize(num_pids);
    for(DWORD i = 0; i < num_pids; ++i) {
        ret->push_back(pids[i]);
    }
    return ret;
}

unsigned long long
WindowsRawProcessCollection::get_process_time_(unsigned int pid) const
{
    HANDLE process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights to process
        false, // should processes created by this process inherit the handle
        pid
    );

    FILETIME ft_creation, ft_exit, ft_kernel, ft_user;
    bool success = GetProcessTimes(
        process_handle,
        &ft_creation,
        &ft_exit,
        &ft_kernel,
        &ft_user
    ) != 0;

    CloseHandle(process_handle);

    if(success) {
        unsigned long long kernel_time =
            convert_filetime_to_ulonglong_(ft_kernel);
        unsigned long long user_time =
            convert_filetime_to_ulonglong_(ft_user);

        unsigned long long process_time = kernel_time + user_time;

        return process_time;
    }
    return 0;
}

unsigned long long 
WindowsRawProcessCollection::get_process_ram_usage_(unsigned int pid) const
{
    HANDLE process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights
        false, // should child processes inherit handle
        pid
    );

    PROCESS_MEMORY_COUNTERS memory_counters;
    bool success = GetProcessMemoryInfo(
        process_handle,
        &memory_counters,
        sizeof(memory_counters)
    ) != 0;

    CloseHandle(process_handle);

    if(success) {
        return memory_counters.WorkingSetSize;
    }
    return 0;
}

void WindowsRawProcessCollection::platform_specific_update_()
{
    // Only way to get PPID in Windows is to use CreateToolhelp32Snapshot
    // and enumerate through each process
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0
    );
    if(snapshot == INVALID_HANDLE_VALUE) {
        return;
    }
    
    PROCESSENTRY32 process;
    process.dwSize = sizeof(process);
    if(!Process32First(snapshot, &process)) {
        CloseHandle(snapshot);
        return;
    }

    do {
        unsigned int pid = process.th32ProcessID;

        ProcessInformation& info = processes_[pid];
        unique_ptr<string> process_name =
            get_process_exe_name_(process.szExeFile);
        info.name = *process_name;
        info.after_time = get_process_time_(pid);
        info.ram_usage = get_process_ram_usage_(pid);

        info.ppid = process.th32ParentProcessID;

        set_last_update_run_(&info, current_update_run_);
    } while(Process32Next(snapshot, &process));

    CloseHandle(snapshot);
}