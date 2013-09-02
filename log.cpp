#include "log.hpp"
#include "array_utilities.hpp"
#include <boost/noncopyable.hpp>

// We deliberately do NOT use "jewel/assert.hpp" here,
// as we might one day want a call the assert to invoke
// logging facilities, and we don't want to create
// circularity here.
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
		explicit StreamHolder(ostream* p_os);
		~StreamHolder();
		void kill();
		ostream* os;
	};

	StreamHolder::StreamHolder(ostream* p_os): os(p_os)
	{
	}

	StreamHolder::~StreamHolder()
	{
		JEWEL_LOG_MESSAGE(Log::info, "Destroying logging StreamHolder.");
		kill();
	}

	void
	StreamHolder::kill()
	{
		try
		{
			if (os)
			{
				os->flush();
				if ((os != &cerr) && (os != &clog) && (os != &cout))
				{
					JEWEL_LOG_MESSAGE
					(	Log::info,
						"Deleting underlying logging stream."
					);
					delete os;
				}
				os = 0;
			}
		}
		catch (...)
		{
			// Yes this is deliberate.
		}
		assert (0 == os);
		return;
	}



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
		assert (osp->good());  // guaranteed by stream_aux().
		assert (!osp->exceptions());  // guaranteed by stream_aux().
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
	// Things get a bit paranoid in here but I think it's worth it to
	// ensure that the mere act of logging doesn't compromise the safety
	// of client code in any way.

	static StreamHolder holder(&std::clog);

	if (p_stream != 0)
	{
		// Then set the stream.
		holder.kill();	
		holder.os = p_stream;
	}

	// If the stream has exceptions enabled, we refuse to work with it.
	if (holder.os && holder.os->exceptions())
	{
		holder.kill();
		assert (0 == holder.os);
	}

	// Make sure the stream isn't in a bad state.
	if (holder.os && !holder.os->good())
	{
		if (!holder.os->bad() && !holder.os->eof())
		{
			assert (holder.os->fail());
			*(holder.os) << "Failbit set on logging stream. Logging "
						 << "discontinued." << endl;
		}
		else
		{
			// We shouldn't write if ios::eof or ios::bad is set.
			// Too dangerous. Do nothing.
		}
		holder.kill();
		assert (0 == holder.os);
	}

	assert
	(	(0 == holder.os) ||
		(holder.os->good() && !holder.os->exceptions())
	);

	return holder.os;
}


}  // namespace jewel
