typedef struct {
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t eip;
	uint32_t mmx_contrl;
	uint32_t x87_control;
} context_t;