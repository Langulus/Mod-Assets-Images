#include "MContent.hpp"
#include "Textures/PNG.hpp"

#define PC_VERBOSE_TEXTURE(a) pcLogSelfVerbose << a

/// Texture generator construction															
///	@param producer - the producer module												
CGeneratorTexture::CGeneratorTexture(MContent* producer)
	: ATexture {MetaData::Of<CGeneratorTexture>()}
	, TProducedFrom {producer} {
	mConstruct = { ClassMeta() };
	ClassValidate();
}

/// Texture destruction																			
CGeneratorTexture::~CGeneratorTexture() {
	Uninitialize();
}

/// Add generators, traits and data to content											
///	@param verb - the creation verb														
void CGeneratorTexture::Create(Verb& verb) {
	if (verb.GetArgument().IsEmpty())
		return;

	// Accumulate stuff into the content construct								
	mConstruct << verb.GetArgument();
	verb.Done();
}

/// Initialize the texture using the descriptor											
void CGeneratorTexture::Generate() {
	if (mGenerated)
		return;

	// Scan constructor																	
	PC_VERBOSE_TEXTURE("Generating " << mConstruct);
	mConstruct.GetAll().ForEachDeep([&](Block& group) {
		EitherDoThis
			group.ForEach([&](AFile& file) {
				PNG::LoadPNG(file, *this);
			})
		OrThis
			group.ForEach([&](const Path& path) {
				auto file = GetRuntime()->GetFile(path);
				if (file) PNG::LoadPNG(*file, *this);
			});
	});

	if (GetDataList().IsEmpty())
		throw Except::Content(pcLogSelfError << "No content was generated");

	PC_VERBOSE_TEXTURE(ccGreen << "Generated");
	mGenerated = true;
}

/// Build the pixel format based on the texture constructor							
void CGeneratorTexture::BuildPixel() {
	mConstruct.GetAll().ForEachDeep([&](const Block& group) {
		group.ForEach([&](const DataID& type) {
			auto meta = type.GetMeta();
			if (!meta->InterpretsAs<AColor>())
				return;
			mView.mFormat = meta;
		});

		group.ForEach([&](const MetaData* meta) {
			if (!meta->InterpretsAs<AColor>())
				return;
			mView.mFormat = meta;
		});
	});
}

/// Fill a preallocated texture with random colors										
///	@param rng - the random number generator											
///	@return true if the texture was filled with noise								
void CGeneratorTexture::FillWithNoise(PCMRNG& rng) {
	GetDataList().Clear();
	Any content;
	
	if (mView.mFormat->GetMemberCount<pcu8>() > 0) {
		content.Allocate<pcu8>(mView.CountBytes());
		for (pcptr i = 0; i < mView.CountBytes(); ++i)
			content << rng.GenerateInt<pcu8>();

		// Calculate ranges and save data											
		switch (mView.mFormat->GetMemberCount<pcu8>()) {
		case 1:
		{
			pcu8 min {std::numeric_limits<pcu8>::max()};
			pcu8 max {std::numeric_limits<pcu8>::min()};
			for (auto i = content.GetRawAs<pcu8>(); i < content.GetRawEndAs<pcu8>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		case 3:
		{
			rgb24 min {rgb24::Max()};
			rgb24 max {rgb24::Min()};
			for (auto i = content.GetRawAs<rgb24>(); i < content.GetRawEndAs<rgb24>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		case 4:
		{
			rgba32 min {rgba32::Max()};
			rgba32 max {rgba32::Min()};
			for (auto i = content.GetRawAs<rgba32>(); i < content.GetRawEndAs<rgba32>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		default:
			throw Except::Content(pcLogSelfError << "Unsupported texture color format");
		}
	}
	else if (mView.mFormat->GetMemberCount<real>() > 0) {
		// For 32bit color components													
		auto components = mView.mFormat->GetMemberCount<real>();
		content.Allocate<real>(mView.CountPixels() * components);
		for (pcptr i = 0; i < mView.CountPixels() * components; ++i)
			content << rng.GenerateRealClosed<real>();

		// Calculate ranges and save data											
		switch (mView.mFormat->GetMemberCount<real>()) {
		case 1:
		{
			real min {std::numeric_limits<real>::max()};
			real max {std::numeric_limits<real>::lowest()};
			for (auto i = content.GetRawAs<real>(); i < content.GetRawEndAs<real>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		case 3:
		{
			rgb96 min {rgb96::Max()};
			rgb96 max {rgb96::Min()};
			for (auto i = content.GetRawAs<rgb96>(); i < content.GetRawEndAs<rgb96>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		case 4:
		{
			rgba128 min {rgba128::Max()};
			rgba128 max {rgba128::Min()};
			for (auto i = content.GetRawAs<rgba128>(); i < content.GetRawEndAs<rgba128>(); ++i) {
				min = pcMin(min, *i);
				max = pcMax(max, *i);
			}
			SaveData<Traits::Color>(pcMove(content), min, max);
			break;
		}
		default:
			throw Except::Content(pcLogSelfError << "Unsupported texture color format");
		}

	}
	else throw Except::Content(pcLogSelfError << "Unsupported texture color format");
}