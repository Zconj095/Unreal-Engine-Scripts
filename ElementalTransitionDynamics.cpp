#include "ElementalTransitionDynamics.h"
#include "Math/Vector2D.h"

double UElementalTransitionDynamics::CalculateLatentHeatEnergy(double Mass, double LatentHeat)
{
	return Mass * LatentHeat;
}

double UElementalTransitionDynamics::PredictLatentHeatEnergy(const FVector2D& Inputs, const FVector2D& Weights)
{
	return Inputs.X * Weights.X + Inputs.Y * Weights.Y;
}

void UElementalTransitionDynamics::Test()
{
	// Example values
	const double Mass = 2.0;
	const double LatentHeat = 334000.0;

	const double Energy = CalculateLatentHeatEnergy(Mass, LatentHeat);
	UE_LOG(LogTemp, Log, TEXT("Calculated Energy (Q): %.0f Joules"), Energy);

	const FVector2D Inputs(Mass, LatentHeat);
	const FVector2D Weights(1.0, 1.0);
	const double PredictedEnergy = PredictLatentHeatEnergy(Inputs, Weights);
	UE_LOG(LogTemp, Log, TEXT("Predicted Energy (Q): %.0f Joules"), PredictedEnergy);
}
