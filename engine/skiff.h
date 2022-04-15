#pragma once

namespace Skiff
{
	// --- Idea stolen from Lumix hehe
	using i8 = char;
	using u8 = unsigned char;
	using i16 = short;
	using u16 = unsigned short;
	using i32 = int;
	using u32 = unsigned int;
	#if defined(_WIN32)
		using i64 = long long;
		using u64 = unsigned long long;
	#else
		using i64 = long;
		using u64 = unsigned long;
	#endif



}
