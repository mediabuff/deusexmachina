#include "ConstantBufferSet.h"

#include <Render/ShaderConstant.h>
#include <Render/GPUDriver.h>

namespace Render
{

bool CConstantBufferSet::SetGPU(PGPUDriver NewGPU)
{
	if (NewGPU == GPU) OK;

	if (GPU.IsValidPtr())
	{
		//???!!!recreate all non-tmp buffers with the new GPU!?
		NOT_IMPLEMENTED;
		UnbindAndClear();
	}

	GPU = NewGPU;
	OK;
}
//---------------------------------------------------------------------

bool CConstantBufferSet::RegisterPermanentBuffer(HConstBuffer Handle, CConstantBuffer& Buffer)
{
	if (Handle == INVALID_HANDLE) FAIL;

	IPTR BufferIdx = Buffers.FindIndex(Handle);
	CConstBufferRecord& Rec = (BufferIdx == INVALID_INDEX) ? Buffers.Add(Handle) : Buffers.ValueAt(BufferIdx);
	Rec.Buffer = &Buffer;
	Rec.Flags = 0;

	OK;
}
//---------------------------------------------------------------------

CConstantBuffer* CConstantBufferSet::RequestBuffer(HConstBuffer Handle, EShaderType Stage)
{
	IPTR BufferIdx = Buffers.FindIndex(Handle);
	CConstBufferRecord& Rec = (BufferIdx == INVALID_INDEX) ? Buffers.Add(Handle) : Buffers.ValueAt(BufferIdx);

	// If buffer is still not used
	if (!Rec.Flags)
	{
		if (Rec.Buffer.IsNullPtr())
		{
			// No permanent buffer registered, create temporary buffer and mark it for later deletion
			Rec.Buffer = GPU->CreateTemporaryConstantBuffer(Handle);
			if (Rec.Buffer.IsNullPtr()) return NULL;
			Rec.Flags |= ECSV_TmpBuffer;
		}

		if (!GPU->BeginShaderConstants(*Rec.Buffer)) return NULL;
	}

	// Mark buffer as used at the Stage
	Rec.Flags |= (1 << Stage);

	return Rec.Buffer.GetUnsafe();
}
//---------------------------------------------------------------------

bool CConstantBufferSet::CommitChanges()
{
	for (UPTR i = 0; i < Buffers.GetCount(); ++i)
	{
		// If buffer contents didn't change, skip it
		// NB: due to Flags structure, it works only for one buffer - one stage
		// scenario, which may change later
		CConstBufferRecord& Rec = Buffers.ValueAt(i);
		if (!Rec.Flags) continue;
		
		CConstantBuffer& Buffer = *Rec.Buffer;

		if (!GPU->CommitShaderConstants(Buffer)) FAIL;

		// Bind buffer to all stages it is used at
		HConstBuffer hCB = Buffers.KeyAt(i);
		for (UPTR j = 0; j < Render::ShaderType_COUNT; ++j)
			if (Rec.Flags & (1 << j))
				GPU->BindConstantBuffer((Render::EShaderType)j, hCB, &Buffer);

		if (Rec.Flags & ECSV_TmpBuffer)
		{
			GPU->FreeTemporaryConstantBuffer(Buffer);
			Rec.Buffer = NULL;
		}

		Rec.Flags = 0;
	}

	OK;
}
//---------------------------------------------------------------------

void CConstantBufferSet::UnbindAndClear()
{
	if (GPU.IsValidPtr())
	{
		//NOT_IMPLEMENTED; //!!!unbind!
	}
	Buffers.Clear(true);
	GPU = NULL;
}
//---------------------------------------------------------------------

}