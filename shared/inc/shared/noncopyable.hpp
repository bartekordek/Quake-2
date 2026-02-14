#pragma once

#define Q2_NONCOPYABLE( TypeName )                  \
public:                                              \
    TypeName( TypeName&& ) = delete;                 \
    TypeName( const TypeName& ) = delete;            \
    TypeName& operator=( const TypeName& ) = delete; \
    TypeName& operator=( TypeName&& ) = delete;
