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
//ȡ�ý���·��
CString GetModulePath();
void  GetModulePath(CString  &Path);
std::string get_module_path();

/* �ļ�������޸�ʱ�� */
int file_last_modified(const char* p_file_name);
int file_last_modified(const wchar_t* p_file_name);

/* �Ƿ�Ϊһ���ļ���(����Ŀ¼Ҳ�������ļ�) */
bool file_exist(const char* p_file_name);
bool file_exist(const wchar_t* p_file_name);
bool is_dir_exist(const wchar_t* p_dir_name);
bool is_dir_exist(const char* p_dir_name);
/* ɾ���ļ� */
bool delete_file(const char* p_file_name);
bool delete_file(const wchar_t* p_file_name);
/* ɾ���ļ�������վ */
bool delete_file_undo(const wchar_t* p_file_name, bool b_file_only = true);

/* ɾ��Ŀ¼ */
bool delete_dir(const wchar_t * p_dir_name);
/* �ж�һ���ַ����Ƿ�Ϊһ��Ŀ¼�� */
bool is_directory(const char* p_file_name);
bool is_directory(const wchar_t* p_file_name);
/* �ж�һ���ַ����Ƿ�Ϊһ���ļ�����������Ŀ¼�� */
bool is_file(const char* p_file_name);

std::vector<std::wstring> get_dir_stack(const std::wstring& s_file_name, bool b_reverse = false);
//rip���ļ���
CString rip_file_name(const CString& s_path_file, bool b_suffix = true);
CString rip_file_ext(const CString& s_path_file, bool b_lower = true);

/* �ƶ�ĳ���ļ���ָ����Ŀ¼ */
bool move_file(const TCHAR* p_src_file, const TCHAR* p_dest_file);
/* ��ȡ�ļ���·��������ֵ�Ľ�β����б�ܣ� */
CString rip_path(const CString& s_file_name);

/*�ļ��Ƿ�ֻ��״̬*/
bool is_read_only(const char* p_file_name);
bool is_read_only(const wchar_t* p_file_name);

/*ȥ�����ļ���ֻ��״̬*/
bool remove_read_only(const char* p_file_name);
bool remove_read_only(const wchar_t* p_file_name);

/* �����ļ� */
bool copy_file(const TCHAR* p_source_file, const TCHAR* p_dest_file, bool b_force = false);

/*	����һ���ļ� */
bool create_file(const TCHAR* p_file_name, bool b_force, unsigned int n_len);

/* ȡ���ļ���С */
unsigned int get_file_size(const char* p_file_name);
unsigned int get_file_size(const wchar_t* p_file_name);

/* ȷ��Ŀ¼���� 
*	˵����
*			���ָ����Ŀ¼�����ڣ���ᴴ����Ŀ¼��
*/
bool confirm_dir(const char* p_dir_name, const char c_splitter = '\\');
bool confirm_dir_2(const char* p_file_name, const char c_splitter = '\\');
bool confirm_dir(const wchar_t* p_dir_name, const wchar_t c_splitter = '\\');
bool confirm_dir_2(const wchar_t* p_file_name, const wchar_t c_splitter = '\\');

static const unsigned short MAX_TIME_BUF = 128;
/* ��ʽ��gmtʱ�� */
void format_time(int n_gmt_time, const char* p_format, char* p_output);
void format_time(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

void format_time_old(int n_gmt_time, const char* p_format, char* p_output);
void format_time_old(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

void format_time_new(int n_gmt_time, const char* p_format, char* p_output);

void init_io();
void init_time_zone();
/* ȡ��gmt��ǰʱ�� */
int now_time(int n_local_time = 0);
/* ȡ��local��ǰʱ�� */
int local_time(int n_gmt_time = 0);

/* ����ʱ����Ϊ��λ��*/
int time_diff(int n_from, int n_to = 0);

/* �ҵ�c_find���ֵ�λ�ã�û���ҵ�����n_len */
unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find);
unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find);


//ȡ���ļ���
void get_file_name(CString &cs_path);
/*	����ȡ������
*	˵����
*			round_up(18, 32) = 32��
*/
template<class T> 
T round_up(T nValue, T nAlign)
{
	return (nValue + nAlign - 1) & ~(nAlign - 1);
}

/* ȡ�ô��̿ռ��С */
__int64 get_disk_space(const TCHAR* p_disk_name);

/* ȡ��ʣ��ռ����Ĵ��� */
CString get_max_space_disk();

/* ȡ��ӵ��Ŀ¼������Ĵ��̣��������ͬ�ģ�ȡ����޸��Ǹ�*/
CString get_have_dir_disk(const TCHAR* p_dir);

/*	ת��������ֵΪʮ������ֵ */
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

/* �滻�������ļ�·���淶���ַ� */
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
	static const unsigned int DEF_TIMEOUT_MILL_SECONDS = MILL_SECONDS_PER_SECOND;	//Ĭ��1�볬ʱ
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
