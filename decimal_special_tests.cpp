#include "decimal.hpp"
#include "decimal_special_tests.hpp"
#include "stopwatch.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using boost::algorithm::split;
using std::accumulate;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;



namespace jewel
{

namespace detail
{

void decimal_speed_test()
{
	int const lim = 1000000;
	vector<Decimal> vec;
	for (int i = 0; i != lim; ++i)
	{
		vec.push_back(Decimal("3.20"));
		vec.push_back(Decimal("98.35"));
	}

	// Base case... we will factor this out when
	// measuring the speed of the arithmetic operations
	Stopwatch sw_base_case;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
	}
	double const base_case = sw_base_case.seconds_elapsed();


	Stopwatch sw_multiplication;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 *= d1;
	}
	cout << lim << " multiplications take "
	     << sw_multiplication.seconds_elapsed() - base_case
	     << " seconds." << endl;

	Stopwatch sw_division;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 /= d1;
	}
	cout << lim << " divisions take "
	     << sw_division.seconds_elapsed() - base_case
		 << " seconds." << endl;

	
	Stopwatch sw_addition;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 += d1;
	}
	cout << lim << " additions take "
	     << sw_addition.seconds_elapsed() - base_case
	     << " seconds." << endl;

		
	Stopwatch sw_subtraction;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 -= d1;
	}
	cout << lim << " subtractions take "
	     << sw_subtraction.seconds_elapsed() - base_case
	     << " seconds." << endl;


	// Measure construction
	int const ctest_lim = 1000000 / 5;
	vector<string> ctest_vec;
	for (int i = 0; i != ctest_lim; ++i)
	{
		ctest_vec.push_back("-0.9871967");
		ctest_vec.push_back("13987.870");
		ctest_vec.push_back("23.2");
		ctest_vec.push_back("19");
		ctest_vec.push_back("-1234.197423");
	}
	Stopwatch sw_ctest_base;
	for (vector<string>::const_iterator it = ctest_vec.begin();
	  it != ctest_vec.end(); ++it)
	{
		string s = *it;
	}
	double const ctest_base_case = sw_ctest_base.seconds_elapsed();
	Stopwatch sw_ctest;
	for (vector<string>::const_iterator it = ctest_vec.begin();
	  it != ctest_vec.end(); ++it)
	{
		Decimal d0(*it);
	}
	cout << ctest_lim * 5 << " calls to Decimal constructor-from-string"
	     << " take " << sw_ctest.seconds_elapsed() - ctest_base_case
		 << " seconds." << endl;

	return;
}


bool is_dagger(char c)
{
	return c == '|';
}


void decimal_csv_test()
{
	cout << "Testing processing two columns of Decimals from a text file..."
	     << endl;
	ifstream data("testdata");
	string current_record;
	Decimal field_0_total("0");
	Decimal field_1_total("0");
	while (getline(data, current_record))
	{
		vector<string> record_vec;
		split(record_vec, current_record, is_dagger);
		Decimal d0(record_vec[0]);
		Decimal d1(record_vec[1]);
		field_0_total += d0;
		field_1_total += d1; 
	}
	if (field_0_total != Decimal("0"))
	{
		cout << "Test failed. First column should have summed to 0, but "
		        "instead summed to: " << field_0_total << endl;
	}
	if (field_1_total != Decimal("0"))
	{
		cout << "Test failed. Second column should have summed to 0, but "
		        "instead summed to: " << field_1_total << endl;
	}
	if (field_0_total == Decimal("0") && field_1_total == Decimal("0"))
	{
		cout << "Test succeeded. Both columns of Decimals summed to 0, as "
		        "expected." << endl;
	}
	return;
}

void decimal_serialization_test()
{
	// Generate sample of Decimals
	
	cout << "Generating 1,000,000 Decimals..." << endl;
	vector<Decimal> vec;
	Decimal const lim("250000");
	for (Decimal dec("0"); dec != lim; ++dec)
	{
		Decimal dec0 = -dec - Decimal("0.45");
		Decimal dec1("23.03");
		Decimal dec2 = dec + Decimal("0.89");
		Decimal dec3 = dec * Decimal("2");
		vec.push_back(dec0);
		vec.push_back(dec1);
		vec.push_back(dec2);
		vec.push_back(dec3);
	}
	cout << "Decimals generated." << endl;
	Decimal const total_as_written =
	  accumulate(vec.begin(), vec.end(), Decimal("0"));


	// Serialize in binary format
	
	ofstream bofs("binary_store.archive");
	boost::archive::binary_oarchive boa(bofs);
	cout << "Writing 1,000,000 Decimals to binary archive..."
	     << endl;
	Stopwatch sw_write_binary;
	for (vector<Decimal>::const_iterator it = vec.begin();
	  it != vec.end(); )
	{
		boa << *it;
		++it;
		boa << *it;
		++it;
		boa << *it;
		++it;
		boa << *it;
		++it;
	}
	bofs.flush();
	bofs.close();
	cout << "Writing to binary archive took "
	     << sw_write_binary.seconds_elapsed() << " seconds."
		 << endl;


	// Serialize in XML format

	ofstream xofs("xml_store.xml");
	boost::archive::xml_oarchive xoa(xofs);
	cout << "Writing 1,000,000 Decimals to XML archive..."
	     << endl;
	Stopwatch sw_write_xml;
	for (vector<Decimal>::const_iterator it = vec.begin();
	  it != vec.end(); )
	{
		Decimal d0 = *it;
		++it;
		Decimal d1 = *it;
		++it;
		Decimal d2 = *it;
		++it;
		Decimal d3 = *it;
		++it;
		xoa << BOOST_SERIALIZATION_NVP(d0)
		    << BOOST_SERIALIZATION_NVP(d1)
			<< BOOST_SERIALIZATION_NVP(d2)
			<< BOOST_SERIALIZATION_NVP(d3);
	}
	xofs.flush();
	xofs.close();
	cout << "Writing to XML archive took "
	     << sw_write_xml.seconds_elapsed() << " seconds."
		 << endl;


	// Read back from binary archive
	
	vector<Decimal> decs_read_back_from_binary_store;
	cout << "Now reading 1,000,000 binary-serialized Decimals from "
	        "binary archive, and pushing them onto a vector..." << endl;
	Stopwatch swbin;
	ifstream bifs("binary_store.archive");
	boost::archive::binary_iarchive bia(bifs);
	for (int i = 0; i != 1000000; ++i)
	{
		Decimal d;
		bia >> d;
		decs_read_back_from_binary_store.push_back(d);
	}
	cout << "Done. Reading from binary archive took "
		 << swbin.seconds_elapsed() << " seconds." << endl;


	// Check integrity of Decimals read back from binary archive

	Decimal total_read_back_from_binary_store =
	  accumulate(decs_read_back_from_binary_store.begin(),
	  decs_read_back_from_binary_store.end(), Decimal("0"));

	if (total_read_back_from_binary_store != total_as_written)
	{
		cout << "Fail! Sum of Decimals read back from binary archive "
		        "does not equal sum as written." << endl;
	}
	else
	{
		cout << "Decimals successfully stored and read back from "
		        "binary archive. Sum of Decimals read back equals "
				"sum of Decimals written." << endl;
	}
	bifs.close();


	// Read back from XML archive

	vector<Decimal> decs_read_back_from_xml_store;
	cout << "Now reading 1,000,000 XML-serialized Decimals from "
	        "XML archive, and pushing them onto a vector..." << endl;
	Stopwatch swxml;
	ifstream xifs("xml_store.xml");
	boost::archive::xml_iarchive xia(xifs);
	for (int i = 0; i != 1000000; ++i)
	{
		Decimal d;
		xia >> BOOST_SERIALIZATION_NVP(d);
		decs_read_back_from_xml_store.push_back(d);
	}
	cout << "Done. Reading from XML archive took "
		 << swxml.seconds_elapsed() << " seconds." << endl;
	

	// Check integrity of Decimals read back from XML archive

	Decimal total_read_back_from_xml_store =
	  accumulate(decs_read_back_from_xml_store.begin(),
	  decs_read_back_from_xml_store.end(), Decimal("0"));

	if (total_read_back_from_xml_store != total_as_written)
	{
		cout << "Fail! Sum of Decimals read back from XML archive "
		        "does not equal sum as written." << endl;
	}
	else
	{
		cout << "Decimals successfully stored and read back from "
		        "XML archive. Sum of Decimals read back equals "
				"sum of Decimals written." << endl;
	}
	xifs.close();

	return;
}

}  // namespace detail

}  // namespace jewel

