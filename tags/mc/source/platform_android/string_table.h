/*
 * string_table.h
 *
 *  Created on: 13.10.2014
 *      Author: borisov.v
 */

#ifndef STRING_TABLE_H_
#define STRING_TABLE_H_

#include "engine.h"

namespace Pegas
{
	typedef String STRINGID;
	class StringTable;

	class StringResource: public BaseResource<STRINGID>
	{
	public:
		StringResource(const StringTable& owner);

		virtual void create(const STRINGID& code) {}
		virtual void load() {}
		virtual void destroy() {}

		String getString();

	private:
		StringTable& m_owner;
	};

	class StringTable
	{
	public:
		bool load(const String& path);
		String getString(const STRINGID& id);
	private:
		typedef std::map<STRINGID, String> STRING_LOOKUP_MAP;
		typedef STRING_LOOKUP_MAP::iterator STRING_LOOKUP_MAP_IT;
		STRING_LOOKUP_MAP m_strings;
	};
}

#endif /* STRING_TABLE_H_ */
