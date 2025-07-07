#pragma once

#include "__internalObjectBase.hpp"

namespace Core
{
	class igMemoryPool;
	class igDataObjectReader;
	class igDataWriterStream;
	class igObjectDirectory;

	class igObject : public __internalObjectBase
	{
	public:
		DefineVirtualFunc_1(bool, isAlikeDeep, igObject*, other)
		DefineVirtualFunc_1(void, userAllocateFields, bool, unk)
		DefineVirtualFunc_0(void, userDeallocateFields)
		DefineVirtualFunc_1(void, userCopyDeep, igObject*, other)
		DefineVirtualFunc_1(void, userCopyShallow, igObject*, other)
		DefineVirtualFunc_1(void, userInstantiate, bool, unk)
		DefineVirtualFunc_0(void, userRelease)
		DefineVirtualFunc_1(void, userResetFields, bool, unk)
		DefineVirtualFunc_0(void, preWrite)
		DefineVirtualFunc_0(void, postWrite)
		DefineVirtualFunc_0(void, preFileWrite)
		DefineVirtualFunc_0(void, postFileWrite)
		DefineVirtualFunc_0(void, postRead)
		DefineVirtualFunc_0(void, postFileRead)
		DefineVirtualFunc_1(void, createDataInheritanceInfo, igMemoryPool*, pool)
		DefineVirtualFunc_1(void, readFromStream, igDataObjectReader*, reader)
		DefineVirtualFunc_1(void, readItemFromStream, igDataObjectReader*, reader)
		DefineVirtualFunc_2(void, writeItemToStream, igDataWriterStream*, stream, igObjectDirectory*, directory)
		DefineVirtualFunc_0(void, clean)
	};
}
