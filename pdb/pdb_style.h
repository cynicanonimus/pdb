#ifndef PDB_STYLE_SETTINGS_SECTION_NAMES_H
#define PDB_STYLE_SETTINGS_SECTION_NAMES_H
//
//
//
#include <QString>

static const QString  g_strCOMPANY       ="CynicSoft";
const static QString  g_str_CNF_APP_NAME = "Pdb";
//
const static QString g_str_ar_SUPPORTED_DB[] = {"QMYSQL", "DoNotUseIt"};
//
const static QString g_str_ar_APP_STYLES[] = {"windows", "motif", "cde", "plastique", "windowsxp", "macintosh"};
//
const static QString g_str_WINDOW_STATE     = "window_state";
const static QString g_str_WINDOW_GEOMETRY  = "window_geometry";
//
//
// Main database settings
//
const static QString  g_str_DB_SETTINGS     = "db/settings";
//
// Color settings
//
const static QString  g_str_CLR_APP_STYLE = "color/appstyle";
//
const static QString  g_str_CLR_NORNAL_FG = "color/normal_foreground_color";
const static QString  g_str_CLR_NORNAL_BG = "color/normal_background_color";
//
const static QString  g_str_CLR_DELETED_FG = "color/deleted_foreground_color";
const static QString  g_str_CLR_DELETED_BG = "color/deleted_background_color";
//
const static QString  g_str_CLR_ATTACH_NORNAL_FG = "color/attach_normal_foreground_color";
const static QString  g_str_CLR_ATTACH_NORNAL_BG = "color/attach_normal_background_color";
//
const static QString  g_str_CLR_ATTACH_DELETED_FG = "color/attach_deleted_foreground_color";
const static QString  g_str_CLR_ATTACH_DELETED_BG = "color/attach_deleted_background_color";
//
const static QString  g_str_CLR_ATTACH_UPDATING_FG = "color/attach_updating_foreground_color";
const static QString  g_str_CLR_ATTACH_UPDATING_BG = "color/attach_updating_background_color";
//
const static QString  g_str_CLR_ATTACH_FAIL_FG = "color/attach_fail_foreground_color";
const static QString  g_str_CLR_ATTACH_FAIL_BG = "color/attach_fail_background_color";
//
// Attachment settings
//
const static QString  g_str_ATTACH_MAX_SIZE_MB  =   "attachment/max_size";
const static QString  g_str_ATTACH_EXPORT_PATH  =   "attachment/export_path";
const static QString  g_str_ATTACH_IMPORT_PATH  =   "attachment/import_path";
const static QString  g_str_ATTACH_TMP_PATH     =   "attachment/tmp_path";
const static QString  g_str_TMP_DELETE          =   "attachment/tmp_delete";
const static QString  g_str_ATTACH_DELETE       =   "attachment/attach_delete";
const static QString  g_str_ATTACH_NODE_DELETE  =   "attachment/attach_node_delete";
//Log settings
const static QString  g_str_LOG_ENABLED         =   "log/enabled";
const static QString  g_str_LOG_PATH            =   "log/path";
const static QString  g_str_LOG_ERROR           =   "log/error";

#endif //PDB_STYLE_SETTINGS_SECTION_NAMES_H
