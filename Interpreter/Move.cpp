#include <cstring>
#include "Move.h"
namespace Interpretator
{
   
        void move::operator()(CPU& cpu) noexcept
        {
            const uint8_t mode = cpu.GetCmd().cmd16.r2;
            const uint8_t r1 = cpu.GetCmd().cmd16.r1;
            const uint8_t r3 = cpu.GetCmd().cmd16.r3;

            Address_t adress = cpu.GetCmd().adress;
            int16_t offset = adress;
            if (mode == 0b000)// ===Регистр - Регистр===
            {
                cpu.reg.Int[r1].int32 = cpu.reg.Int[r3].int32;
            }
            else
            {
                cpu.isLongCmd = 1;
                switch (mode)
                {
                    // ===Регистр - Память===
                case 0b001: //1 -- адрес (константа) в команде
                    std::memcpy(&cpu.reg.Int[r1].word[0], &cpu.memory[adress], sizeof(Word_t) * 2);
                    break;
                case 0b011: //3 -- адрес = r3 + константа (в команде) 
                    offset += cpu.reg.Int[r3].int32;
                    std::memcpy(&cpu.reg.Int[r1].word[0], &cpu.memory[offset], sizeof(Word_t) * 2);
                    break;
                    // ===Память - Регистр===
                case 0b101://5
                    std::memcpy(&cpu.memory[adress], &cpu.reg.Int[r1].word[0], sizeof(Word_t) * 2);
                    break;
                case 0b111://7
                    offset += cpu.reg.Int[r3].int32;
                    std::memcpy(&cpu.memory[offset], &cpu.reg.Int[r1].word[0], sizeof(Word_t) * 2);
                    break;
                    // ===Загрузка адреса в регистр===
                case 0b010://2
                    cpu.reg.Int[r1].word[0] = adress;
                    cpu.reg.Int[r1].word[1] = 0;
                    break;
                case 0b110://6
                    cpu.reg.Int[r1].word[0] = (int16_t)offset + cpu.reg.Int[r3].int32;
                    cpu.reg.Int[r1].word[1] = 0;
                    break;
                }
            }
        }
}
