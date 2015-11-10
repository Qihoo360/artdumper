#include "stdafx.h"
#include "OATHeader.h"

namespace Art {
	const uint8_t OATHeader::kOatMagic[] = { 'o', 'a', 't', '\n' };
	const uint8_t OATHeader::kOatVersion[] = { '0', '4', '6', '\0' };
	static constexpr int kPageSize = 4096;

	template<int n, typename T>
	static inline bool IsAligned(T x) {
		static_assert((n & (n - 1)) == 0, "n is not a power of two");
		return (x & (n - 1)) == 0;
	}

	template<int n, typename T>
	static inline bool IsAligned(T* x) {
		return IsAligned<n>(reinterpret_cast<const uintptr_t>(x));
	}

	bool OATHeader::IsValid() const{
		if (memcmp(magic_, kOatMagic, sizeof(kOatMagic)) != 0) {
			return false;
		}

		if (!IsAligned<kPageSize>(executable_offset_)) {
			return false;
		}

		if (!IsAligned<kPageSize>(image_patch_delta_)) {
			return false;
		}

		return true;
	}

	const char* OATHeader::GetMagic() const {
		return reinterpret_cast<const char*>(magic_);
	}

	uint32_t OATHeader::GetChecksum() const{
		return adler32_checksum_;
	}

}