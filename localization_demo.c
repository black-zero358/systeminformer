/*
 * SystemInformer Language Demonstration
 * 
 * This file demonstrates how the localization system works
 * and shows examples of translated content.
 */

#include <phapp.h>
#include <phlang.h>
#include <phstring_res.h>

// Example function showing how to use localized strings
VOID PhShowLocalizedExample(VOID)
{
    PPH_STRING localizedText;
    
    // Get current language information
    wprintf(L"Current Language: %s (%s)\n", 
        PhGetLanguageName(PhCurrentLanguage),
        PhGetLanguageNativeName(PhCurrentLanguage));
    
    // Example of getting localized strings
    localizedText = PhGetLocalizedString(IDS_GENERAL);
    if (localizedText)
    {
        wprintf(L"General tab: %s\n", localizedText->Buffer);
        PhDereferenceObject(localizedText);
    }
    
    localizedText = PhGetLocalizedString(IDS_PROCESS);
    if (localizedText)
    {
        wprintf(L"Process: %s\n", localizedText->Buffer);
        PhDereferenceObject(localizedText);
    }
    
    localizedText = PhGetLocalizedString(IDS_SYSTEM_INFORMATION);
    if (localizedText)
    {
        wprintf(L"System Information: %s\n", localizedText->Buffer);
        PhDereferenceObject(localizedText);
    }
}

// Example showing language switching
VOID PhDemonstrateLanguageSwitching(VOID)
{
    wprintf(L"\n=== Language Switching Demonstration ===\n");
    
    // Show English
    PhSetCurrentLanguage(PhLanguageEnglish);
    wprintf(L"English: %s\n", PhGetLanguageNativeName(PhCurrentLanguage));
    PhShowLocalizedExample();
    
    // Show Chinese
    PhSetCurrentLanguage(PhLanguageChineseSimplified);
    wprintf(L"Chinese: %s\n", PhGetLanguageNativeName(PhCurrentLanguage));
    PhShowLocalizedExample();
}

// Example showing auto-detection
VOID PhDemonstrateAutoDetection(VOID)
{
    PH_LANGUAGE_ID detectedLanguage;
    
    wprintf(L"\n=== Auto-Detection Demonstration ===\n");
    
    detectedLanguage = PhDetectSystemLanguage();
    wprintf(L"Detected system language: %s (%s)\n", 
        PhGetLanguageName(detectedLanguage),
        PhGetLanguageNativeName(detectedLanguage));
        
    if (detectedLanguage == PhLanguageChineseSimplified)
    {
        wprintf(L"System is using Chinese, switching to Chinese interface.\n");
    }
    else
    {
        wprintf(L"System is not using Chinese, keeping English interface.\n");
    }
}

/*
 * Expected Output Examples:
 * 
 * === English Mode ===
 * Current Language: English (English)
 * General tab: General
 * Process: Process
 * System Information: System Information
 * 
 * === Chinese Mode ===
 * Current Language: Chinese (Simplified) (简体中文)
 * General tab: 常规
 * Process: 进程
 * System Information: 系统信息
 * 
 * === Dialog Examples ===
 * 
 * English Dialog Titles:
 * - General
 * - Modules  
 * - Threads
 * - Handles
 * - Environment
 * - About
 * - Run As
 * - System Information
 * 
 * Chinese Dialog Titles:
 * - 常规
 * - 模块
 * - 线程
 * - 句柄
 * - 环境
 * - 关于
 * - 运行身份
 * - 系统信息
 * 
 * === Button Examples ===
 * 
 * English Buttons:
 * - OK, Cancel, Apply, Browse, Search, Copy, Save, Close
 * - Permissions, Terminate, Refresh, Options, Inspect, Open
 * 
 * Chinese Buttons:
 * - 确定、取消、应用、浏览、搜索、复制、保存、关闭
 * - 权限、终止、刷新、选项、检查、打开
 * 
 * === Status Messages ===
 * 
 * English:
 * - Ready, Loading..., Processing..., Complete, Error
 * - Access denied, File not found, Operation failed
 * 
 * Chinese:
 * - 就绪、加载中...、处理中...、完成、错误
 * - 访问被拒绝、找不到文件、操作失败
 */