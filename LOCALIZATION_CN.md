# SystemInformer 简体中文本地化实现说明

## 概述

本项目为 SystemInformer (系统信息器) 实现了高质量的简体中文本地化支持。通过创建完整的多语言基础架构和全面的中文资源文件，使中文用户能够以母语使用这一强大的系统监控工具。

## 实现架构

### 1. 多语言支持架构

#### 核心组件
- **语言管理器** (`phlang.h`, `phlang.c`): 提供语言检测、切换和资源管理功能
- **中文资源文件** (`SystemInformer_zh-CN.rc`): 包含所有中文界面元素
- **字符串资源表** (`phstring_res.h`): 定义常用字符串的资源ID

#### 设计特点
- **动态语言切换**: 支持运行时语言选择和切换
- **自动语言检测**: 根据系统语言自动选择合适的界面语言
- **资源优化**: 按需加载语言资源，节省内存占用
- **向后兼容**: 完全兼容原有的英文界面

### 2. 语言支持API

```c
// 语言类型定义
typedef enum _PH_LANGUAGE_ID
{
    PhLanguageEnglish = 0,
    PhLanguageChineseSimplified = 1,
    PhLanguageMax
} PH_LANGUAGE_ID;

// 主要功能函数
VOID PhInitializeLanguageSupport(VOID);
BOOLEAN PhSetCurrentLanguage(PH_LANGUAGE_ID LanguageId);
PH_LANGUAGE_ID PhDetectSystemLanguage(VOID);
PPH_STRING PhGetLocalizedString(ULONG StringId);
```

## 本地化内容

### 1. 对话框界面 (15+ 个完整对话框)

#### 进程管理相关
- **进程常规信息**: 文件信息、进程信息、命令行等
- **进程模块**: 模块列表和管理
- **进程线程**: 线程信息和控制
- **进程句柄**: 句柄列表和属性
- **进程环境**: 环境变量查看
- **进程性能**: CPU、内存、I/O 性能图表
- **进程统计**: 详细统计信息
- **进程内存**: 内存区域管理

#### 系统管理相关
- **系统信息**: 全面的系统信息显示
- **处理器关联**: CPU 关联性设置
- **运行身份**: 以不同用户身份运行程序
- **创建服务**: 系统服务创建和配置

#### 通用界面
- **关于对话框**: 软件信息和版权说明
- **进度对话框**: 操作进度显示
- **信息对话框**: 通用信息展示
- **选择对话框**: 选项选择界面
- **运行对话框**: 快速运行程序
- **查找对话框**: 句柄和DLL查找

#### 设置和选项
- **常规选项**: 包含语言选择和基本设置
- **高级选项**: 专业用户设置
- **突出显示**: 颜色和显示设置

### 2. 字符串资源 (75+ 个常用字符串)

#### 界面元素
- 按钮文本: 确定、取消、应用、重置、浏览等
- 标签文本: 常规、模块、线程、句柄、环境等
- 状态信息: 就绪、加载中、处理中、完成、错误等

#### 错误消息
- 访问被拒绝、文件未找到、参数无效等
- 内存不足、操作失败等系统错误

#### 进程相关术语
- 进程名、进程ID、内存、CPU、线程数、句柄数、状态等

#### 系统信息术语
- 系统信息、CPU使用率、内存使用率、磁盘使用率、网络使用率等

### 3. 选项界面集成

在程序的选项设置中添加了完整的语言选择功能：

- **语言选择下拉框**: 显示所有可用语言的本地化名称
- **自动检测选项**: 根据系统语言自动选择界面语言
- **即时生效**: 语言更改后提示重启以应用新设置

## 技术实现细节

### 1. 资源文件结构

```rc
// 中文资源文件结构
#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED

// 对话框资源
IDD_PROCGENERAL DIALOGEX 0, 0, 260, 260
STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "常规"
...

// 字符串表
STRINGTABLE
BEGIN
    IDS_GENERAL         "常规"
    IDS_MODULES         "模块"
    ...
END
```

### 2. 语言检测机制

```c
PH_LANGUAGE_ID PhDetectSystemLanguage(VOID)
{
    LANGID systemLangId = GetSystemDefaultUILanguage();
    
    // 精确匹配
    for (ULONG i = 0; i < PhLanguageMax; i++)
    {
        if (PhLanguageTable[i].LangId == systemLangId)
            return PhLanguageTable[i].LanguageId;
    }
    
    // 主语言匹配
    LANGID primaryLang = MAKELANGID(PRIMARYLANGID(systemLangId), SUBLANG_DEFAULT);
    for (ULONG i = 0; i < PhLanguageMax; i++)
    {
        if (PRIMARYLANGID(PhLanguageTable[i].LangId) == PRIMARYLANGID(primaryLang))
            return PhLanguageTable[i].LanguageId;
    }
    
    return PhLanguageEnglish; // 默认英文
}
```

### 3. 设置保存机制

语言设置被保存到用户配置文件中：

```c
// 设置键定义
#define PH_SETTING_NAME_LANGUAGE L"Language"
#define PH_SETTING_NAME_AUTO_DETECT_LANGUAGE L"AutoDetectLanguage"

// 初始化时读取设置
if (PhGetIntegerSetting(L"AutoDetectLanguage"))
{
    languageId = PhDetectSystemLanguage();
}
else
{
    languageId = (PH_LANGUAGE_ID)PhGetIntegerSetting(L"Language");
}
```

## 翻译质量标准

### 1. 术语统一性
- 使用 Microsoft Windows 官方中文术语
- 保持技术术语的准确性和一致性
- 遵循计算机专业术语的标准翻译

### 2. 界面适配
- 考虑中文文本的长度特点
- 保持界面布局的美观和实用性
- 确保所有控件都能正确显示中文文本

### 3. 用户体验
- 使用简洁明了的中文表达
- 避免生硬的直译，注重语言的自然性
- 保持与原有功能的完全一致性

## 使用方法

### 1. 编译安装
本地化功能已集成到主项目中，使用标准的编译流程即可：

```bash
# 编译项目
cd build
build_release.cmd
```

### 2. 语言设置
1. 打开 SystemInformer
2. 进入 "选项" -> "常规"
3. 在 "显示语言" 中选择 "简体中文"
4. 可选择启用 "自动检测语言"
5. 重启应用程序以应用新语言设置

### 3. 自动检测
如果启用了自动检测功能，程序会根据系统语言自动选择合适的界面语言。

## 扩展指南

### 添加新的对话框翻译
1. 在 `SystemInformer_zh-CN.rc` 中添加对话框定义
2. 翻译所有文本元素
3. 确保控件ID与原版一致

### 添加新的字符串资源
1. 在 `phstring_res.h` 中定义新的字符串ID
2. 在资源文件的字符串表中添加翻译
3. 在代码中使用 `PhGetLocalizedString()` 获取字符串

### 添加新语言支持
1. 在 `PH_LANGUAGE_ID` 枚举中添加新语言
2. 在 `PhLanguageTable` 中添加语言信息
3. 创建对应的资源文件
4. 更新项目文件包含新资源

## 总结

本项目成功实现了 SystemInformer 的全面中文本地化，包括：

- **完整的技术架构**: 从底层语言管理到上层用户界面的全方位支持
- **高质量的翻译**: 覆盖了主要功能界面的准确、专业的中文翻译
- **用户友好的体验**: 简单易用的语言选择和自动检测功能
- **良好的扩展性**: 便于添加更多语言和界面的本地化支持

这一实现为中文用户提供了原生的使用体验，同时为其他语言的本地化工作提供了完整的参考框架。