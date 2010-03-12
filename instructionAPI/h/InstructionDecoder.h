/*
 * Copyright (c) 1996-2009 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if !defined(INSTRUCTION_DECODER_H)
#define INSTRUCTION_DECODER_H

#include "InstructionAST.h"
#include "Expression.h"
#include "Operation.h"
#include "Operand.h"
#include "Instruction.h"
#include "dyn_regs.h"

#include <vector>
#include <dyn_detail/boost/enable_shared_from_this.hpp>

    
namespace Dyninst
{
  namespace InstructionAPI
  {
      class InstructionDecoder;
      static const unsigned int maxInstructionLength = 16;
    dyn_detail::boost::shared_ptr<InstructionDecoder> makeDecoder(Architecture arch, const unsigned char* buffer, unsigned len =
        maxInstructionLength);
    /// The %InstructionDecoder class decodes instructions, given a buffer of bytes and a length,
    /// and constructs an %Instruction.
    /// The %InstructionDecoder will, by default, be constructed to decode machine language
    /// on the platform on which it has been compiled.  The buffer
    /// will be treated as if there is an instruction stream starting at the beginning of the buffer.
    /// %InstructionDecoder objects are given a buffer from which to decode at construction.
    /// Calls to \c decode will proceed to decode instructions sequentially from that buffer until its
    /// end is reached.  At that point, all subsequent calls to \c decode will return an invalid
    /// %Instruction object.
    ///
    /// An %InstructionDecoder object may alternately be constructed without designating a buffer,
    /// and the buffer may be specified at the time \c decode is called.  This method of use may be
    /// more convenient for users who are decoding non-contiguous instructions.

    class InstructionDecoder : public dyn_detail::boost::enable_shared_from_this<InstructionDecoder>
    {
      friend class Instruction;
        protected:
      /// Construct an %InstructionDecoder object that decodes from \c buffer, up to \c size bytes.
      INSTRUCTION_EXPORT InstructionDecoder(const unsigned char* buffer, size_t size, Architecture arch);
      
      /// Construct an %InstructionDecoder object with no buffer specified.
      INSTRUCTION_EXPORT InstructionDecoder();
      
        public:
            typedef dyn_detail::boost::shared_ptr<InstructionDecoder> Ptr;
            friend Ptr makeDecoder(Architecture arch,
                                   const unsigned char* buffer, unsigned len);
            INSTRUCTION_EXPORT virtual ~InstructionDecoder();
        private:
      INSTRUCTION_EXPORT InstructionDecoder(const InstructionDecoder& o);
      /// Decode the current instruction in this %InstructionDecoder object's buffer, interpreting it as 
      /// machine language of the type understood by this %InstructionDecoder.
      /// If the buffer does not contain a valid instruction stream, a null %Instruction pointer
      /// will be returned.  The %Instruction's \c size field will contain the size of the instruction decoded.
        public:
      INSTRUCTION_EXPORT virtual Instruction::Ptr decode();
      /// Decode the instruction at \c buffer, interpreting it as machine language of the type
      /// understood by this %InstructionDecoder.  If the buffer does not contain a valid instruction stream, 
      /// a null %Instruction pointer will be returned.  The %Instruction's \c size field will contain
      /// the size of the instruction decoded.
      INSTRUCTION_EXPORT virtual Instruction::Ptr decode(const unsigned char* buffer);
      
      INSTRUCTION_EXPORT virtual void setMode(bool is64) = 0;

      virtual void doDelayedDecode(const Instruction* insn_to_complete) = 0;
      void setBuffer(const unsigned char* buffer, unsigned int size = 0);
      void resetBuffer();
    protected:
      
      virtual bool decodeOperands(const Instruction* insn_to_complete) = 0;

      virtual unsigned int decodeOpcode() = 0;
      
      virtual Expression::Ptr makeAddExpression(Expression::Ptr lhs, Expression::Ptr rhs, Result_Type resultType);
      virtual Expression::Ptr makeMultiplyExpression(Expression::Ptr lhs, Expression::Ptr rhs, Result_Type resultType);
      virtual Expression::Ptr makeDereferenceExpression(Expression::Ptr addrToDereference, Result_Type resultType);
      virtual Expression::Ptr makeRegisterExpression(MachRegister reg);
      virtual Result_Type makeSizeType(unsigned int opType) = 0;
      Instruction* makeInstruction(entryID opcode, const char* mnem, unsigned int decodedSize,
              const unsigned char* raw);
      
    protected:
      Operation::Ptr m_Operation;
      const unsigned char* bufferBegin;
      size_t bufferSize;
      const unsigned char* rawInstruction;
      const unsigned char* oldBufferBegin;
      size_t oldBufferSize;
      const unsigned char* oldBuffer;
      Architecture m_Arch;
      
    };
  };
};

#endif //!defined(INSTRUCTION_DECODER_H)
