using System.Data;
using Internal;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BrainFuckInterpreter
{
    internal class Program
    {

        /*
            -------------------- BRAINFUCK COMMANDS --------------------
            [ > ]       Moves the pointer to the right
            [ < ]       Moves the pointer to the left
            [ + ]       Increments the memory cell at the pointer
            [ - ]       Decremenets the memory cell at the pointer
            [ . ]       Output the char in the cell pointed to by the pointer
            [ , ]       Input a char and store it in the cell at the pointer
            [ [ ]       Jump past the matching ] if the cell at the pointer is 0
            [ ] ]       Jump back to the matching [ if the cell at the pointer != 0
            -------------------- BRAINFUCK COMMANDS --------------------
        */

        private static byte[] _stack;
        private static int _ptr;
        private static char[] _input;

        static void Main(string[] args)
        {
            SetupInterpreter();
            Console.ReadLine();
        }

        private static void SetupInterpreter()
        {
            //Console.Clear();
            Console.WriteLine("Enter file path (make sure to include .bf): ");
            try
            {
                _input = File.ReadAllText(Console.ReadLine()).ToCharArray();
                _stack = new byte[Byte.MaxValue];
                _ptr = 0;

                RunBrainfuck();
            }
            catch { Console.WriteLine("File not found"); SetupInterpreter(); }
        }
        
        private static void RunBrainfuck()
        {
            int unmatchedBrackets = 0;

            for (int i = 0; i < _input.Length; i++)
            {
                switch (_input[i])
                {
                    case '>':
                        _ptr++;
                        break;
                    case '<':
                        _ptr--;
                        break;
                    case '+':
                        _stack[_ptr]++;
                        break;
                    case '-':
                        _stack[_ptr]--;
                        break;
                    case '.':
                        Console.Write(Convert.ToChar(_stack[_ptr]));
                        break;
                    case ',':
                        var key = Console.ReadKey();
                        _stack[_ptr] = (byte)key.KeyChar;
                        break;
                    case '[':
                        if (_stack[_ptr] == 0)
                        {
                            unmatchedBrackets++;
                            while (_input[i] != ']' || unmatchedBrackets != 0)
                            {
                                i++;

                                if (_input[i] == '[')
                                    unmatchedBrackets++;
                                else if (_input[i] == ']')
                                    unmatchedBrackets--;
                            }
                        }
                        break;
                    case ']':
                        if (_stack[_ptr] != 0)
                        {
                            unmatchedBrackets++;
                            while (_input[i] != '[' || unmatchedBrackets != 0)
                            {
                                i--;

                                if (_input[i] == ']')
                                    unmatchedBrackets++;
                                else if (_input[i] == '[')
                                    unmatchedBrackets--;
                            }
                        }
                        break;
                }
            }
        }
    }
}