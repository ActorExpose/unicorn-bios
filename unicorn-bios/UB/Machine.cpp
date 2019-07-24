/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Jean-David Gadina - www.xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/Interrupts.hpp"

namespace UB
{
    class Machine::IMPL
    {
        public:
            
            IMPL( size_t memory, const FAT::Image & fat );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            void _setup( const Machine & machine );
            
            size_t     _memory;
            FAT::Image _fat;
            Engine     _engine;
    };

    Machine::Machine( size_t memory, const FAT::Image & fat ):
        impl( std::make_unique< IMPL >( memory, fat ) )
    {
        this->impl->_setup( *( this ) );
    }

    Machine::Machine( const Machine & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {
        this->impl->_setup( *( this ) );
    }

    Machine::Machine( Machine && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    Machine::~Machine( void )
    {}

    Machine & Machine::operator =( Machine o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    bool Machine::start( void )
    {
        return this->impl->_engine.start( 0x7C00 );
    }
    
    void swap( Machine & o1, Machine & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    Machine::IMPL::IMPL( size_t memory, const FAT::Image & fat ):
        _memory( memory ),
        _fat(    fat ),
        _engine( memory )
    {}

    Machine::IMPL::IMPL( const IMPL & o ):
        _memory( o._memory ),
        _fat(    o._fat ),
        _engine( o._memory )
    {}

    Machine::IMPL::~IMPL( void )
    {}
    
    void Machine::IMPL::_setup( const Machine & machine )
    {
        this->_engine.onInterrupt
        (
            [ & ]( uint32_t i, Engine & engine ) -> bool
            {
                switch( i )
                {
                    case 0x05: Interrupts::int0x05( machine, engine ); break;
                    case 0x10: Interrupts::int0x10( machine, engine ); break;
                    case 0x11: Interrupts::int0x11( machine, engine ); break;
                    case 0x12: Interrupts::int0x12( machine, engine ); break;
                    case 0x13: Interrupts::int0x13( machine, engine ); break;
                    case 0x14: Interrupts::int0x14( machine, engine ); break;
                    case 0x15: Interrupts::int0x15( machine, engine ); break;
                    case 0x16: Interrupts::int0x16( machine, engine ); break;
                    case 0x17: Interrupts::int0x17( machine, engine ); break;
                    case 0x18: Interrupts::int0x18( machine, engine ); break;
                    case 0x19: Interrupts::int0x19( machine, engine ); break;
                    case 0x1A: Interrupts::int0x1A( machine, engine ); break;
                    
                    default: return false;
                }
                
                return true;
            }
        );
    }
}