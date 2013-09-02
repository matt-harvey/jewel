#include "log.hpp"
#include "array_utilities.hpp"
#include <boost/noncopyable.hpp>
#include <cassert>
#include <iostream>

using boost::noncopyable;
using jewel::num_elements;
using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::ostream;


namespace jewel
{

namespace
{

struct StreamHolder: noncopyable
{
	explicit StreamHolder(ostream* p_os): m_os(p_os)
	{
	}
	~StreamHolder()
	{
		JEWEL_LOG_MESSAGE(Log::info, "Destroying logging StreamHolder.");
		kill();
	}
	void kill()
	{
		assert (m_os);
		m_os->flush();
		if ((m_os != &cerr) && (m_os != &clog) && (m_os != &cout))
		{
			JEWEL_LOG_MESSAGE
			(	Log::info,
				"Deleting underlying logging stream."
			);
			delete m_os;
		}
		m_os = 0;
		return;
	}
	ostream* m_os;
};


}  // end anonymous namespace

void
Log::set_stream(ostream* p_os)
{
	stream_aux(p_os);
	return;
}

void
Log::set_threshold(Level p_level)
{
	threshold_aux() = p_level;
	return;
}

void
Log::log
(	Level p_severity,
	std::string const& p_message,
	std::string const& p_function,
	std::string const& p_file,
	int p_line,
	std::string const& p_compilation_date,
	std::string const& p_compilation_time
)
{
	log
	(	p_severity,
		p_message.c_str(),
		p_function.c_str(),
		p_file.c_str(),
		p_line,
		p_compilation_date.c_str(),
		p_compilation_time.c_str()
	);
	return;
}

void
Log::log
(	Level p_severity,
	char const* p_message,
	char const* p_function,
	char const* p_file,
	int p_line,
	char const* p_compilation_date,
	char const* p_compilation_time
)
{
	if (p_severity >= threshold_aux())
	{
		ostream* const osp = stream_aux();
		if (!osp)
		{
			return;
		}
		*osp << "{\n"
			<< "\tseverity: \"" << severity_string(p_severity) << "\"\n"
			<< "\tmessage: \"" << p_message << "\",\n"
			<< "\tfunction: \"" << p_function << "\",\n"
			<< "\tfile: \"" << p_file << "\",\n"
			<< "\tline: " << p_line << ",\n"
			<< "\tcompilation_date: \"" << p_compilation_date << "\",\n"
			<< "\tcompilation_time: \"" << p_compilation_time << "\"\n"
			<< "},"
			<< endl;
	}
	return;
}

char const*
Log::severity_string(Level p_level)
{
	static char const* strings[] =
	{	"trace",
		"info",
		"warning",
		"error"
	};
	if (p_level >= num_elements(strings))
	{
		return "unrecognized";
	}
	return strings[p_level];
}

std::ostream*
Log::stream_aux(std::ostream* p_stream)
{
	static StreamHolder holder(&std::clog);
	if (p_stream)
	{
		holder.kill();	
		holder.m_os = p_stream;
	}
	assert (holder.m_os);
	return holder.m_os;
}


}  // namespace jewel
