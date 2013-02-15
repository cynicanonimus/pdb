#ifndef PDB_STYLE_SETTINGS_SECTION_NAMES_H
#define PDB_STYLE_SETTINGS_SECTION_NAMES_H
//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
#include <QString>

#ifdef Q_WS_WIN
static const QString  g_strPATH_DELIMETER = "\\";
#else
static const QString  g_strPATH_DELIMETER = "/";
#endif

static const QString  g_strCOMPANY       ="CynicSoft";
const static QString  g_str_CNF_APP_NAME = "Pdb";
//
const static QString g_str_ar_APP_STYLES[] = {"windows", "motif", "cde", "plastique", "windowsxp", "macintosh"};
//
const static QString g_str_WINDOW_STATE     = "window_state";
const static QString g_str_WINDOW_GEOMETRY  = "window_geometry";
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
const static QString  g_str_DELETE_AFTER_UPLOAD =   "attachment/attach_delete_after_upload";
//Log settings
const static QString  g_str_LOG_ENABLED         =   "log/enabled";
const static QString  g_str_LOG_PATH            =   "log/path";
const static QString  g_str_LOG_ERROR           =   "log/error";
//
const static QString  g_str_LOG_FLAGS           =   "log/flags";
//--------------------------------------------------------------------------
const static QString  g_str_SEC_PROTECT     =   "security/protect";
//
const static QString  g_str_SEC_TEC_CODE    =   "security/tec_code";
//
const static QString  g_str_SEC_SCHRED      =   "security/schred";
const static QString  g_str_SEC_SCHRED_CMD  =   "security/schred_cmd";
//
const static QString  g_str_SEC_MARK_CRYPT  =   "security/mark_crypt";
//
enum LogNotificationCodes
{
    en_ENABLE_LOG,
    en_LOG_ERRORS,
    //--------------------------------------------
    en_LOG_CREATE_NODE,
    en_LOG_UPDATE_NODE,
    en_LOG_DELETE_NODE,
    en_LOG_MOVE_NODE,
    en_LOG_EXPORT_NODE,
    en_LOG_DESCRIPTOR_NODE,
    //--------------------------------------------
    en_LOG_CREATE_ATTACH,
    en_LOG_DELETE_ATTACH,
    en_LOG_UPDATE_ATTACH, //en_LOG_UPDATE_ATTACH
    en_LOG_RENAME_ATTACH,
    en_LOG_MOVE_ATTACH,
    en_LOG_VIEW_ATTACH,
    en_LOG_EXPORT_ATTACH,
    en_LOG_CRYPT_ATTACH,
    //--------------------------------------------
    en_LOG_CREATE_TREE,
    en_LOG_DELETE_TREE,
    en_LOG_RENAME_TREE,
    //------- all new flags - after this line -------------------
    //------- all new flags - before this line -------------------
    en_BITARRAY_SIZE
};

#endif //PDB_STYLE_SETTINGS_SECTION_NAMES_H
