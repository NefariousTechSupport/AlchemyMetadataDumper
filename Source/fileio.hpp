#pragma once


#include <stdint.h>

// extra level of indirection to so the line number is used,
// otherwise __LINE__ would be tokenized
#define WriteFormattedTextConcat_(a, b) a ## b
#define WriteFormattedTextConcat(a, b) WriteFormattedTextConcat_(a, b)
#define WriteFormattedText(writer, fmt, ...) \
	len = __stubs::__sprintf(buf, fmt, __VA_ARGS__); \
	writer.WriteText(len, buf);

#define WriteIndentation(writer, indentation) \
	for (int indenterI = 0; indenterI < (indentation); indenterI++) \
	{ \
		writer.WriteText(1, "\t"); \
	}
#define WriteFormattedTextIndented(writer, indentation, fmt, ...) \
	WriteIndentation(writer, indentation) \
	WriteFormattedText(writer, fmt, __VA_ARGS__);

#define WriteIndentedText(writer, indentation, len, text) \
	WriteIndentation(writer, indentation) \
	writer.WriteText((len), (text));

class FileWriter
{
public:
	explicit FileWriter(const char* filepath);
	~FileWriter();

	int WriteText(int len, const char* text);
	int WriteTextf(int n, const char* fmt, ...);

private:
	void platformOpenFile(const char* filepath);
	void platformTruncateFile(const char* filepath);
	void platformCloseHandle();
	int32_t platformWriteFile(const char* data, int32_t len);
	int _handle;
};