#ifndef __COMPILERFP_H__
#define __COMPILERFP_H__

class CParser;

struct fragment_program_exec
{
	u32 data[4];
};

struct fragment_program_data
{
	u32 offset;
	s32 index;
	s32 user;
};

class CCompilerFP
{
public:
	CCompilerFP();
	virtual ~CCompilerFP();

	void Compile(CParser *pParser);

	int GetNumRegs() const { return m_nNumRegs; }
	int GetFPControl() const { return m_nFPControl; }
	int GetTexcoords() const { return m_nTexcoords; }
	int GetTexcoord2D() const { return m_nTexcoord2D; }
	int GetTexcoord3D() const { return m_nTexcoord3D; }

	int GetInstructionCount() const {return m_nInstructions;}
	std::list<struct fragment_program_data> GetConstRelocations() const { return m_lConstData; }
	struct fragment_program_exec* GetInstructions() const { return m_pInstructions; }

private:
	void Prepare(CParser *pParser);

	void emit_insn(struct nvfx_insn *insn,u8 op);
	void emit_dst(struct nvfx_insn *insn,bool *have_const);
	void emit_src(struct nvfx_insn *insn,s32 pos,bool *have_const);
	void emit_brk(struct nvfx_insn *insn);
	void emit_rep(struct nvfx_insn *insn);
	void emit_if(struct nvfx_insn *insn);
	void emit_loop(struct nvfx_insn *insn);
	void emit_lrp(struct nvfx_insn *insn);
	void emit_pow(struct nvfx_insn *insn);
	void emit_lit(struct nvfx_insn *insn);
	void emit_ddx(struct nvfx_insn *insn);
	void emit_ddy(struct nvfx_insn *insn);
	void emit_txl(struct nvfx_insn *insn);
	void emit_tex(struct nvfx_insn *insn,u8 op);
	void fixup_rep();
	void fixup_loop();
	void fixup_if();
	void fixup_else();

	int grow_insns(int count);

	struct nvfx_reg temp();
	void release_temps();

	struct nvfx_reg imm(f32 x, f32 y, f32 z, f32 w);

	inline param GetImmData(int index)
	{
		s32 i;
		std::list<param>::iterator it = m_lParameters.begin();
		for(;it!=m_lParameters.end();it++) {
			for(i=0;i<it->count;i++) {
				if((int)(it->index + i)==index) {
					if(it->is_const && it->is_internal)
						return *it;
				}
			}
		}
		return param();
	}

	inline param GetInputAttrib(int index)
	{
		s32 i;
		std::list<param>::iterator it = m_lParameters.begin();
		for(;it!=m_lParameters.end();it++) {
			for(i=0;i<it->count;i++) {
				if((int)(it->index + i)==index) {
					if(!it->is_const && !it->is_internal)
						return *it;
				}
			}
		}
		return param();
	}

	int m_nNumRegs;
	int m_nFPControl;
	int m_nSamplers;
	int m_nTexcoords;
	int m_nTexcoord2D;
	int m_nTexcoord3D;
	int m_nInstructions;
	int m_nCurInstruction;
	struct fragment_program_exec *m_pInstructions;

	int m_rTemps;
	int m_rTempsDiscard;

	std::list<param> m_lParameters;
	std::list<struct fragment_program_data> m_lConstData;
	std::stack<int> m_repStack;
	std::stack<int> m_ifStack;
	std::stack<int> m_loopStack;
};

#endif
