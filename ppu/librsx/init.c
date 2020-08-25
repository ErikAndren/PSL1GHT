#include <rsx/mm.h>
#include <rsx/gcm_sys.h>
#include <ppu-asm.h>

gcmContextData *gGcmContext ATTRIBUTE_PRXPTR = NULL;

static gcmContextData sUserContext =
{
	NULL,
	NULL,
	NULL,
	NULL
};

extern s32 gcmInitBodyEx(gcmContextData* ATTRIBUTE_PRXPTR *ctx,const u32 cmdSize,const u32 ioSize,const void *ioAddress);

s32 rsxInit(gcmContextData **context,u32 cmdSize,u32 ioSize,const void *ioAddress)
{
	s32 ret = -1;

	ret = gcmInitBodyEx(&gGcmContext,cmdSize,ioSize,ioAddress);
	if(ret==0) {
		rsxHeapInit();

		if (context)
			*context = gGcmContext;
	}
	return ret;
}

void rsxSetupContextData(gcmContextData *context,const u32 *addr,u32 size,gcmContextCallback cb)
{
	u32 alignedSize = size&~0x3;

	context->begin = (u32*)addr;
	context->current = (u32*)addr;
	context->end = (u32*)(addr + alignedSize - 4);
	context->callback = (gcmContextCallback)__get_opd32(cb);
}

void rsxSetCurrentBuffer(gcmContextData **context,const u32 *addr,u32 size)
{
	u32 alignedSize = size&~0x3;
	
	gGcmContext = &sUserContext;

	sUserContext.begin = (u32*)addr;
	sUserContext.current = (u32*)addr;
	sUserContext.end = (u32*)((u64)addr + alignedSize - 4);

	*context = gGcmContext;
}

void rsxSetDefaultCommandBuffer(gcmContextData **context)
{
	gcmSetDefaultCommandBuffer();
	*context = gGcmContext;
}

void rsxSetUserCallback(gcmContextCallback cb)
{
	sUserContext.callback = cb;
}

u32* rsxGetCurrentBuffer()
{
	return gGcmContext->current;
}

