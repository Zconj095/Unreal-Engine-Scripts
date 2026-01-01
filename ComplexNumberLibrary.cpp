#include "ComplexNumberLibrary.h"

#include "Math/UnrealMathUtility.h"

FComplexNumber UComplexNumberLibrary::Create(double Real, double Imaginary)
{
	return FComplexNumber(Real, Imaginary);
}

FComplexNumber UComplexNumberLibrary::Add(const FComplexNumber& A, const FComplexNumber& B)
{
	return FComplexNumber(A.Real + B.Real, A.Imaginary + B.Imaginary);
}

FComplexNumber UComplexNumberLibrary::Multiply(const FComplexNumber& A, const FComplexNumber& B)
{
	return FComplexNumber(
		A.Real * B.Real - A.Imaginary * B.Imaginary,
		A.Real * B.Imaginary + A.Imaginary * B.Real);
}

FComplexNumber UComplexNumberLibrary::DivideByScalar(const FComplexNumber& A, double Scalar)
{
	if (ensureMsgf(!FMath::IsNearlyZero(Scalar), TEXT("Cannot divide complex number by zero scalar.")))
	{
		const double SafeScalar = Scalar;
		return FComplexNumber(A.Real / SafeScalar, A.Imaginary / SafeScalar);
	}

	return FComplexNumber(A.Real, A.Imaginary);
}

double UComplexNumberLibrary::Magnitude(const FComplexNumber& A)
{
	return A.GetMagnitude();
}

FString UComplexNumberLibrary::ComplexToString(const FComplexNumber& A)
{
	return A.ToString();
}
