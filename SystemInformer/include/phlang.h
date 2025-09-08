/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Language Support Header
 *
 */

#ifndef PHLANG_H
#define PHLANG_H

#ifdef __cplusplus
extern "C" {
#endif

// Language identifiers
typedef enum _PH_LANGUAGE_ID
{
    PhLanguageEnglish = 0,
    PhLanguageChineseSimplified = 1,
    PhLanguageMax
} PH_LANGUAGE_ID;

// Language information structure
typedef struct _PH_LANGUAGE_INFO
{
    PH_LANGUAGE_ID LanguageId;
    LANGID LangId;
    PCWSTR Name;
    PCWSTR NativeName;
    PCWSTR LocaleCode;
} PH_LANGUAGE_INFO, *PPH_LANGUAGE_INFO;

// Global language support variables
extern PH_LANGUAGE_ID PhCurrentLanguage;
extern HINSTANCE PhLanguageResourceInstance;

// Function declarations
VOID PhInitializeLanguageSupport(VOID);
VOID PhCleanupLanguageSupport(VOID);
BOOLEAN PhSetCurrentLanguage(PH_LANGUAGE_ID LanguageId);
PH_LANGUAGE_ID PhDetectSystemLanguage(VOID);
HINSTANCE PhLoadLanguageResourceDll(PH_LANGUAGE_ID LanguageId);
VOID PhUnloadLanguageResourceDll(VOID);
PPH_STRING PhGetLocalizedString(ULONG StringId);
PCWSTR PhGetLanguageName(PH_LANGUAGE_ID LanguageId);
PCWSTR PhGetLanguageNativeName(PH_LANGUAGE_ID LanguageId);

// Resource loading macros
#define PhLoadLocalizedString(id) PhGetLocalizedString(id)
#define PhLoadLocalizedIcon(id) LoadIcon(PhLanguageResourceInstance, MAKEINTRESOURCE(id))
#define PhLoadLocalizedBitmap(id) LoadBitmap(PhLanguageResourceInstance, MAKEINTRESOURCE(id))

// Settings keys for language preferences
#define PH_SETTING_NAME_LANGUAGE L"Language"
#define PH_SETTING_NAME_AUTO_DETECT_LANGUAGE L"AutoDetectLanguage"

#ifdef __cplusplus
}
#endif

#endif