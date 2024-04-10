#pragma once

/*enum ReflectionType
{
	DIFF,
	SPEC,
	REFR,
	GLOSSY,
	TRANSL,
	EMIT
};*/

namespace BxDF
{

	enum class Event
	{
		None,
		Diffuse,
		Dirac,
		Emission
/*		Specular,
		Refract,
		Enter,
		Exit,
		Glossy,
		Translucient,
		Emission,
		Dirac = Specular | Refract*/
	};

};
