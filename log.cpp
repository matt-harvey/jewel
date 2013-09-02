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
		kill();
	}
	void kill()
	{
		assert (m_os);
		m_os->flush();
		if ((m_os != &cerr) && (m_os != &clog) && (m_os != &cout))
		{
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
	std::string const& p_file,
	int p_line
)
{
	log(p_severity, p_message.c_str(), p_file.c_str(), p_line);
	return;
}

void
Log::log
(	Level p_severity,
	char const* p_message,
	char const* p_file,
	int p_line
)
{
	if (p_severity >= threshold_aux())
	{
		ostream& os = *(stream_aux());
		os << "{\"severity\": \"" << severity_string(p_severity)
			<< "\"";
		if (p_message)
		{
			os << ", \"message\": \"" << p_message << "\"";
		}
		if (p_file)
		{
			os << ", \"file\": \"" << p_file << "\"";
		}
		if (p_line != -1)
		{
			os << ", \"line\": " << p_line;
		}
		os << "}," << endl;
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
