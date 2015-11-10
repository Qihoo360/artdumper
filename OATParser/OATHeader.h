#pragma once

namespace Art {
	enum InstructionSet {
		kNone,
		kArm,
		kArm64,
		kThumb2,
		kX86,
		kX86_64,
		kMips,
		kMips64
	};

#pragma pack(4)
	class OATHeader
	{
	public:
		OATHeader() {};
		~OATHeader() {};

		static const uint8_t kOatMagic[4];
		static const uint8_t kOatVersion[4];

		//static constexpr const char* kImageLocationKey = "image-location";
		//static constexpr const char* kDex2OatCmdLineKey = "dex2oat-cmdline";
		//static constexpr const char* kDex2OatHostKey = "dex2oat-host";
		//static constexpr const char* kPicKey = "pic";

		bool IsValid() const;
		const char* GetMagic() const;
		uint32_t GetChecksum() const;

		uint32_t GetDexFileCount() const {
			return dex_file_count_;
		}

		uint32_t GetExecutableOffset() const {
			return executable_offset_;
		}

		uint32_t GetKeyValueStoreSize() const {
			return key_value_store_size_;
		}

	private:
		friend class OATParser;
		uint8_t magic_[4];
		uint8_t version_[4];
		unsigned int adler32_checksum_;

		InstructionSet instruction_set_;
		uint32_t instruction_set_features_bitmap_;
		uint32_t dex_file_count_;
		uint32_t executable_offset_;
		uint32_t interpreter_to_interpreter_bridge_offset_;
		uint32_t interpreter_to_compiled_code_bridge_offset_;
		uint32_t jni_dlsym_lookup_offset_;
		uint32_t portable_imt_conflict_trampoline_offset_;
		uint32_t portable_resolution_trampoline_offset_;
		uint32_t portable_to_interpreter_bridge_offset_;
		uint32_t quick_generic_jni_trampoline_offset_;
		uint32_t quick_imt_conflict_trampoline_offset_;
		uint32_t quick_resolution_trampoline_offset_;
		uint32_t quick_to_interpreter_bridge_offset_;

		int32_t image_patch_delta_;

		uint32_t image_file_location_oat_checksum_;
		uint32_t image_file_location_oat_data_begin_;

		uint32_t key_value_store_size_;
		uint8_t key_value_store_[0];  // note variable width data at end

		//DISALLOW_COPY_AND_ASSIGN(OATHeader);
	};
#pragma pack()

}

