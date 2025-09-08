/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Language Support Implementation
 *
 */

#include <phapp.h>
#include <phlang.h>

// Global variables
PH_LANGUAGE_ID PhCurrentLanguage = PhLanguageEnglish;
HINSTANCE PhLanguageResourceInstance = NULL;

// Language information table
static PH_LANGUAGE_INFO PhLanguageTable[PhLanguageMax] = 
{
    { PhLanguageEnglish, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), L"English", L"English", L"en-US" },
    { PhLanguageChineseSimplified, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), L"Chinese (Simplified)", L"简体中文", L"zh-CN" }
};

/**
 * Initializes the language support system.
 */
VOID PhInitializeLanguageSupport(VOID)
{
    PH_LANGUAGE_ID languageId;
    
    // Try to get saved language preference
    if (PhGetIntegerSetting(PH_SETTING_NAME_AUTO_DETECT_LANGUAGE))
    {
        languageId = PhDetectSystemLanguage();
    }
    else
    {
        languageId = (PH_LANGUAGE_ID)PhGetIntegerSetting(PH_SETTING_NAME_LANGUAGE);
        if (languageId >= PhLanguageMax)
            languageId = PhLanguageEnglish;
    }
    
    PhSetCurrentLanguage(languageId);
}

/**
 * Cleans up language support resources.
 */
VOID PhCleanupLanguageSupport(VOID)
{
    PhUnloadLanguageResourceDll();
}

/**
 * Sets the current UI language.
 *
 * \param LanguageId The language ID to set.
 * \return TRUE if successful, FALSE otherwise.
 */
BOOLEAN PhSetCurrentLanguage(PH_LANGUAGE_ID LanguageId)
{
    HINSTANCE resourceInstance;
    
    if (LanguageId >= PhLanguageMax)
        return FALSE;
    
    // Unload current resource DLL if any
    PhUnloadLanguageResourceDll();
    
    // Load new resource DLL
    resourceInstance = PhLoadLanguageResourceDll(LanguageId);
    if (!resourceInstance)
        return FALSE;
    
    PhCurrentLanguage = LanguageId;
    PhLanguageResourceInstance = resourceInstance;
    
    // Save language preference
    PhSetIntegerSetting(PH_SETTING_NAME_LANGUAGE, LanguageId);
    
    return TRUE;
}

/**
 * Detects the system default language.
 *
 * \return The detected language ID.
 */
PH_LANGUAGE_ID PhDetectSystemLanguage(VOID)
{
    LANGID systemLangId;
    ULONG i;
    
    systemLangId = GetSystemDefaultUILanguage();
    
    // Find matching language in our table
    for (i = 0; i < PhLanguageMax; i++)
    {
        if (PhLanguageTable[i].LangId == systemLangId)
            return PhLanguageTable[i].LanguageId;
    }
    
    // Check primary language only
    systemLangId = MAKELANGID(PRIMARYLANGID(systemLangId), SUBLANG_DEFAULT);
    for (i = 0; i < PhLanguageMax; i++)
    {
        if (PRIMARYLANGID(PhLanguageTable[i].LangId) == PRIMARYLANGID(systemLangId))
            return PhLanguageTable[i].LanguageId;
    }
    
    // Default to English
    return PhLanguageEnglish;
}

/**
 * Loads a language resource DLL.
 *
 * \param LanguageId The language ID.
 * \return Handle to the resource DLL, or NULL if failed.
 */
HINSTANCE PhLoadLanguageResourceDll(PH_LANGUAGE_ID LanguageId)
{
    HINSTANCE resourceInstance;
    WCHAR resourcePath[MAX_PATH];
    WCHAR fileName[MAX_PATH];
    
    if (LanguageId >= PhLanguageMax)
        return NULL;
    
    // For English, use the main executable
    if (LanguageId == PhLanguageEnglish)
    {
        return PhInstanceHandle;
    }
    
    // For other languages, try to load separate resource DLL
    if (GetModuleFileName(PhInstanceHandle, resourcePath, RTL_NUMBER_OF(resourcePath)))
    {
        PWSTR baseName;
        
        baseName = PhGetBaseName(resourcePath);
        if (baseName)
        {
            // Create language-specific filename (e.g., SystemInformer_zh-CN.dll)
            PhSwprintf(fileName, RTL_NUMBER_OF(fileName), L"%s_%s.dll", 
                baseName->Buffer, PhLanguageTable[LanguageId].LocaleCode);
            
            PhDereferenceObject(baseName);
            
            // Get directory path
            PWSTR lastSlash = wcsrchr(resourcePath, L'\\');
            if (lastSlash)
            {
                *lastSlash = L'\0';
                PhSwprintf(resourcePath, RTL_NUMBER_OF(resourcePath), L"%s\\%s", 
                    resourcePath, fileName);
                
                resourceInstance = LoadLibraryEx(resourcePath, NULL, 
                    LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
                
                if (resourceInstance)
                    return resourceInstance;
            }
        }
    }
    
    // Fallback: try to use main executable with different language resources
    return PhInstanceHandle;
}

/**
 * Unloads the current language resource DLL.
 */
VOID PhUnloadLanguageResourceDll(VOID)
{
    if (PhLanguageResourceInstance && PhLanguageResourceInstance != PhInstanceHandle)
    {
        FreeLibrary(PhLanguageResourceInstance);
    }
    
    PhLanguageResourceInstance = NULL;
}

/**
 * Gets a localized string resource.
 *
 * \param StringId The string resource ID.
 * \return A string object, or NULL if the string was not found.
 */
PPH_STRING PhGetLocalizedString(ULONG StringId)
{
    PWSTR buffer;
    ULONG length;
    
    if (!PhLanguageResourceInstance)
        PhLanguageResourceInstance = PhInstanceHandle;
    
    length = LoadString(PhLanguageResourceInstance, StringId, (PWSTR)&buffer, 0);
    
    if (length != 0)
    {
        return PhCreateStringEx(buffer, length * sizeof(WCHAR));
    }
    
    return NULL;
}

/**
 * Gets the display name of a language.
 *
 * \param LanguageId The language ID.
 * \return The display name, or NULL if invalid.
 */
PCWSTR PhGetLanguageName(PH_LANGUAGE_ID LanguageId)
{
    if (LanguageId >= PhLanguageMax)
        return NULL;
    
    return PhLanguageTable[LanguageId].Name;
}

/**
 * Gets the native name of a language.
 *
 * \param LanguageId The language ID.
 * \return The native name, or NULL if invalid.
 */
PCWSTR PhGetLanguageNativeName(PH_LANGUAGE_ID LanguageId)
{
    if (LanguageId >= PhLanguageMax)
        return NULL;
    
    return PhLanguageTable[LanguageId].NativeName;
}