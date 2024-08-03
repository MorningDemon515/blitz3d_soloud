
#include "std.h"
#include "bbsys.h"
#include "../gxruntime/gxutf8.h"
#include <time.h>

#define CHKPOS(x) if( (x)<0 ) RTEX( "parameter must be positive" );
#define CHKOFF(x) if( (x)<=0 ) RTEX( "parameter must be greater than 0" );

BBStr *bbString( BBStr *s,int n ){
	BBStr *t=d_new BBStr();
	while( n-->0 ) *t+=*s;
	delete s;return t;
}

BBStr *bbLeft( BBStr *s,int n ){
	CHKPOS( n );
	*s=UTF8::substr(*s, 0,n );return s;
}

BBStr *bbRight( BBStr *s,int n ){
	CHKPOS( n );
	n=UTF8::length(*s)-n;if( n<0 ) n=0;
	*s=UTF8::substr( *s,n,s->size()-n );return s;
}

BBStr *bbReplace( BBStr *s,BBStr *from,BBStr *to ){
	int n=0,from_sz=from->size(),to_sz=to->size();
	while( n<s->size() && (n=s->find( *from,n ))!=string::npos ){
		s->replace( n,from_sz,*to );
		n+=to_sz;
	}
	delete from;delete to;return s;
}

int bbInstr( BBStr *s,BBStr *t,int from ){
	CHKOFF( from );--from;
	int n=UTF8::find(*s, *t,from );
	delete s;delete t;
	return n<0 ? 0 : n+1;
}

BBStr *bbMid( BBStr *s,int o,int n ){
	CHKOFF( o );--o;
	if( o>s->size() ) o=s->size();
	if( n>=0 ) *s=UTF8::substr( *s,o,n );
	else *s=UTF8::substr( *s,o,s->size()-o );
	return s;
}

BBStr *bbUpper( BBStr *s ){
	for( int k=0;k<s->size();++k ) (*s)[k]=toupper( (*s)[k] );
	return s;
}

BBStr *bbLower( BBStr *s ){
	for( int k=0;k<s->size();++k ) (*s)[k]=tolower( (*s)[k] );
	return s;
}

bool isgraph_safe(int chr) {
	if (chr > 127) { return true; }
	return isgraph(chr);
}

BBStr* bbTrim(BBStr* s) {
	int n = 0;
	int p = s->size();
	int c;
	// currently all characters above the standard ASCII range are simply not trimmed
	while( n<s->size() && !isgraph_safe( (unsigned char)(*s)[n] ) ) ++n;
	while( p>n && !isgraph_safe( (unsigned char)(*s)[p-1] ) ) --p;
	*s = UTF8::substr(*s, n, p - n);
	return s;
}

BBStr *bbLSet( BBStr *s,int n ){
	CHKPOS(n);
	if( s->size()>n ) *s=s->substr( 0,n );
	else{
		while( s->size()<n ) *s+=' ';
	}
	return s;
}

BBStr *bbRSet( BBStr *s,int n ){
	CHKPOS(n);
	if( s->size()>n ) *s=s->substr( s->size()-n );
	else{
		while( s->size()<n ) *s=' '+*s;
	}
	return s;
}

BBStr *bbChr( int n ){
	BBStr *t=d_new BBStr();
	*t+=(char)n;return t;
}

BBStr *bbHex( int n ){
	char buff[12];
	for( int k=7;k>=0;n>>=4,--k ){
		int t=(n&15)+'0';
		buff[k]=t>'9' ? t+='A'-'9'-1 : t;
	}
	buff[8]=0;
	return d_new BBStr( buff );
}

BBStr *bbBin( int n ){
	char buff[36];
	for( int k=31;k>=0;n>>=1,--k ){
		buff[k]=n&1 ? '1' : '0';
	}
	buff[32]=0;
	return d_new BBStr( buff );
}

int bbAsc( BBStr *s ){
	int n=s->size() ? (*s)[0] & 255 : -1;
	delete s;return n;
}

int bbLen( BBStr *s ){
	int n=UTF8::length(*s);
	delete s;return n;
}

BBStr *bbCurrentDate(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%d %b %Y",localtime( &t ) );
	return d_new BBStr( buff );
}

BBStr *bbCurrentTime(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%H:%M:%S",localtime( &t ) );
	return d_new BBStr( buff );
}

bool string_create(){
	return true;
}

bool string_destroy(){
	return true;
}

void string_link( void(*rtSym)(const char*,void*) ){
	rtSym( "$String$string%repeat",bbString );
	rtSym( "$Left$string%count",bbLeft );
	rtSym( "$Right$string%count",bbRight );
	rtSym( "$Replace$string$from$to",bbReplace );
	rtSym( "%Instr$string$find%from=1",bbInstr );
	rtSym( "$Mid$string%start%count=-1",bbMid );
	rtSym( "$Upper$string",bbUpper );
	rtSym( "$Lower$string",bbLower );
	rtSym( "$Trim$string",bbTrim );
	rtSym( "$LSet$string%size",bbLSet );
	rtSym( "$RSet$string%size",bbRSet );
	rtSym( "$Chr%ascii",bbChr );
	rtSym( "%Asc$string",bbAsc );
	rtSym( "%Len$string",bbLen );
	rtSym( "$Hex%value",bbHex );
	rtSym( "$Bin%value",bbBin );
	rtSym( "$CurrentDate",bbCurrentDate );
	rtSym( "$CurrentTime",bbCurrentTime );
}
