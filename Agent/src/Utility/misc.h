#pragma once
#include <string>
#include <atlstr.h>
#include <vector>

#define  _WSTAT_DEFINED
#define	_WDIRECT_DEFINED
namespace em_utility
{
namespace misc
{
 CString GetSystemName();
 CString GetSystemVersionMark();
 DWORD GetDllVersion(LPCTSTR lpszDllName);  
 BOOL IsAdmin();

 CString Htmlspecialchars(LPCTSTR pBuf);
 
 BOOL IsValidEmailChar(TCHAR ch);
 BOOL IsValidEmail(CString strEmail);
//取得进程路径
CString GetModulePath();
void  GetModulePath(CString  &Path);
std::string get_module_path();

/* 文件的最后修改时间 */
int file_last_modified(const char* p_file_name);
int file_last_modified(const wchar_t* p_file_name);

/* 是否为一个文件名(包括目录也可以是文件) */
bool file_exist(const char* p_file_name);
bool file_exist(const wchar_t* p_file_name);
bool is_dir_exist(const wchar_t* p_dir_name);
bool is_dir_exist(const char* p_dir_name);
/* 删除文件 */
bool delete_file(const char* p_file_name);
bool delete_file(const wchar_t* p_file_name);
/* 删除文件到回收站 */
bool delete_file_undo(const wchar_t* p_file_name, bool b_file_only = true);

/* 删除目录 */
bool delete_dir(const wchar_t * p_dir_name);
/* 判断一个字符串是否为一个目录名 */
bool is_directory(const char* p_file_name);
bool is_directory(const wchar_t* p_file_name);
/* 判断一个字符串是否为一个文件名（不包括目录） */
bool is_file(const char* p_file_name);

std::vector<std::wstring> get_dir_stack(const std::wstring& s_file_name, bool b_reverse = false);
//rip出文件名
CString rip_file_name(const CString& s_path_file, bool b_suffix = true);
CString rip_file_ext(const CString& s_path_file, bool b_lower = true);

/* 移动某个文件到指定的目录 */
bool move_file(const TCHAR* p_src_file, const TCHAR* p_dest_file);
/* 萃取文件的路径（返回值的结尾不含斜杠） */
CString rip_path(const CString& s_file_name);

/*文件是否只读状态*/
bool is_read_only(const char* p_file_name);
bool is_read_only(const wchar_t* p_file_name);

/*去掉的文件的只读状态*/
bool remove_read_only(const char* p_file_name);
bool remove_read_only(const wchar_t* p_file_name);

/* 复制文件 */
bool copy_file(const TCHAR* p_source_file, const TCHAR* p_dest_file, bool b_force = false);

/*	创建一个文件 */
bool create_file(const TCHAR* p_file_name, bool b_force, unsigned int n_len);

/* 取得文件大小 */
unsigned int get_file_size(const char* p_file_name);
unsigned int get_file_size(const wchar_t* p_file_name);

/* 确认目录存在 
*	说明：
*			如果指定的目录不存在，则会创建该目录。
*/
bool confirm_dir(const char* p_dir_name, const char c_splitter = '\\');
bool confirm_dir_2(const char* p_file_name, const char c_splitter = '\\');
bool confirm_dir(const wchar_t* p_dir_name, const wchar_t c_splitter = '\\');
bool confirm_dir_2(const wchar_t* p_file_name, const wchar_t c_splitter = '\\');

static const unsigned short MAX_TIME_BUF = 128;
/* 格式化gmt时间 */
void format_time(int n_gmt_time, const char* p_format, char* p_output);
void format_time(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

void format_time_old(int n_gmt_time, const char* p_format, char* p_output);
void format_time_old(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

void format_time_new(int n_gmt_time, const char* p_format, char* p_output);

void init_io();
void init_time_zone();
/* 取得gmt当前时间 */
int now_time(int n_local_time = 0);
/* 取得local当前时间 */
int local_time(int n_gmt_time = 0);

/* 计算时间差（秒为单位）*/
int time_diff(int n_from, int n_to = 0);

/* 找到c_find出现的位置，没有找到返回n_len */
unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find);
unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find);


//取得文件名
void get_file_name(CString &cs_path);
/*	向上取整函数
*	说明：
*			round_up(18, 32) = 32。
*/
template<class T> 
T round_up(T nValue, T nAlign)
{
	return (nValue + nAlign - 1) & ~(nAlign - 1);
}

/* 取得磁盘空间大小 */
__int64 get_disk_space(const TCHAR* p_disk_name);

/* 取得剩余空间最大的磁盘 */
CString get_max_space_disk();

/* 取得拥有目录层次最多的磁盘，如果有相同的，取最后修改那个*/
CString get_have_dir_disk(const TCHAR* p_dir);

/*	转换二进制值为十六进制值 */
std::string binary_2_hex(const unsigned char* p_data, unsigned int n_len);
CString _binary_2_hex(const unsigned char* p_data, unsigned int n_len);

CString utf8_2_CString(const char* p_input);
std::string unicode_2_utf8(const wchar_t* p_input);
std::string unicode_2_ansi(const wchar_t* p_input);
std::string utf8_2_ansi(const char* p_input);
std::string ansi_2_utf8(const char* p_input);
CString ansi_2_CString(const char* p_input);

void confirm_splash(CString& s_input, bool b_tail = true);
void confirm_n_splash(CString& s_input, bool b_tail = true);
CString link_dir_splash(const CString& s_base, const CString& s_sub);

/* 替换不符合文件路径规范的字符 */
CString fs_replace(const CString& s_input, const TCHAR c_new = _T('_'));

void erase_html_item(std::wstring& s_data);
void shift_html_item(std::wstring& s_data);

struct ver_info
{
	unsigned int n_major_1;
	unsigned int n_major_2;
	unsigned int n_minor_1;
	unsigned int n_minor_2;
	std::string version;
};

ver_info get_version_info(const char* p_app_name);
ver_info get_version_info(const wchar_t* p_module_name);
int   compare_version(const ver_info& ver1,const ver_info& ver2);

class time_counter
{
public:
	static const unsigned int MILL_SECONDS_PER_SECOND = 1000;
	static const unsigned int DEF_TIMEOUT_MILL_SECONDS = MILL_SECONDS_PER_SECOND;	//默认1秒超时
	time_counter(unsigned int n_mill_seconds = DEF_TIMEOUT_MILL_SECONDS);
	void begin();
	bool end();
	unsigned int timeout() const;
	void tick_it();
	std::string print_tick_list() const;
private:
	typedef std::vector<DWORD> tick_container;
	typedef tick_container::iterator tick_iter;
	typedef tick_container::const_iterator c_tick_iter;
	tick_container m_tick_list;
	unsigned int mn_mill_seconds;
};

class handle_dir
{
public:
	typedef void (*PHANDLEFILE)(const CString& s_file_name);
	handle_dir();
	void run(PHANDLEFILE p_handler, const CString& s_root);
private:
	void check_dir(const CString& s_cur_dir);
	PHANDLEFILE mp_handle;
	CString ms_root;
};

struct url_encoder
{
public:
	static unsigned char conv_2_hex(const unsigned char x)
	{
		return x > 9 ? x + 55: x + 48;
	}
	static std::string url_encoding(const std::string& s_input);
};

}		//end namespace misc

#define DECLARE_ENUM(E) \
struct E \
{ \
public: \
	E(int value = 0) : _value((__Enum)value) { \
} \
	E& operator=(int value) { \
	this->_value = (__Enum)value; \
	return *this; \
} \
	operator int() const { \
	return this->_value; \
} \
	\
	enum __Enum {

#define END_ENUM() \
}; \
	\
private: \
	__Enum _value; \
};

}		//end namespace em_utility
