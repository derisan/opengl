#pragma once

class GLObject
{ 
public:
	GLObject( ) = default;
	virtual ~GLObject( ) = default;

	virtual void Bind( ) const abstract;
	virtual void Unbind( ) const abstract;

protected:
	unsigned int ObjectID = { 0 };
};

