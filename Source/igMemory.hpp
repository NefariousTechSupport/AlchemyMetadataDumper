#pragma once

namespace Core
{
	template<typename T>
	struct igMemory
	{
		auint32_t _size;
		T* _buffer;

		inline T get(int index) const
		{
			return _buffer[index];
		}
		inline void set(int index, T value)
		{
			_buffer[index] = value;
		}
	};
}