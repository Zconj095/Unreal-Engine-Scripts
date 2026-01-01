#include "FriendshipEvaluatorLibrary.h"

static void AddCheck(bool bFlag, const TCHAR* Label, int32& OutMet, int32& OutTotal, TArray<FString>& Met, TArray<FString>& Missing)
{
    OutTotal += 1;
    if (bFlag) { OutMet += 1; Met.Add(Label); }
    else { Missing.Add(Label); }
}

FFriendEvaluation UFriendshipEvaluatorLibrary::EvaluateGoodFriend(const FGoodFriendObservations& Obs, float RequiredPercent)
{
    FFriendEvaluation Result;

    int32 Met = 0, Total = 0;

    // Acceptance & understanding
    AddCheck(Obs.bAcceptsPastPresentFuture, TEXT("Accepts you across past/present/future"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bUnderstandsAndAcceptsYou, TEXT("Understands you and still accepts you"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bAcceptsStrengthsWeaknessesResistances, TEXT("Accepts strengths, weaknesses, resistances"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bAcceptsDisciplinesBeliefsAndChanges, TEXT("Accepts beliefs/discipline and changes over time"), Met, Total, Result.Met, Result.Missing);

    // Shared life & continuity
    AddCheck(Obs.bBuildsLastingMemoriesOverTime, TEXT("Builds lasting memories over time"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bBuildsHobbiesTogether, TEXT("Builds hobbies together"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bReachesOutStaysInTouchOverYears, TEXT("Reaches out and stays in touch"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bLongLastingNotShortComing, TEXT("Long-lasting, not short-coming"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bMakesTimeNotExcuses, TEXT("Makes time, not excuses"), Met, Total, Result.Met, Result.Missing);

    // Character
    AddCheck(Obs.bGoodAndTrueFriend, TEXT("Good and true friend"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bHonest, TEXT("Honest"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bDoesNotLie, TEXT("Doesn't lie"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bNotMoneyMotivated, TEXT("Values friendship over money"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bRealAndTrue, TEXT("Real and true"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bSeesPastManipulativeLanguage, TEXT("Sees past manipulative language"), Met, Total, Result.Met, Result.Missing);

    // Care & protection
    AddCheck(Obs.bProtective, TEXT("Protective"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bLooksAfterYou, TEXT("Looks after you"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bHasYourBackWhenTough, TEXT("Has your back when tough"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bCaresAndFeelsWithYou, TEXT("Cares and feels what you feel"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bUnderstandsToughTimesInstinctively, TEXT("Senses tough times without words"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bEncouragesNaturalHealing, TEXT("Encourages natural healing (supportive)"), Met, Total, Result.Met, Result.Missing);

    // Boundaries & respect
    AddCheck(Obs.bDoesNotPursueSexButAcceptsAllOfYou, TEXT("Respects boundaries; accepts all of you"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bGetsAlongWithWife, TEXT("Gets along with your wife"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bShouldBeLikeYourWifeInLoyaltySupport, TEXT("Wife-like loyalty/support (respectful)"), Met, Total, Result.Met, Result.Missing);

    // Growth & positivity
    AddCheck(Obs.bEncouragesPositiveChemistryHabits, TEXT("Encourages healthy, positive habits"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bInspiresDreamsAndEncourages, TEXT("Inspires and encourages your dreams"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bEncouragesBeingBetterDaily, TEXT("Encourages being better than yesterday"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bEncouragesPositiveNatureAndOvercomesNegativity, TEXT("Fosters positive nature; overcomes negativity"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bSeesYourBrillianceAndEncourages, TEXT("Sees your brilliance and nurtures it"), Met, Total, Result.Met, Result.Missing);

    // Companionship & communication
    AddCheck(Obs.bEnjoysYourCompany, TEXT("Enjoys your company"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bCommunicatesOftenLooksForward, TEXT("Communicates often; looks forward to it"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bChecksOnYouOften, TEXT("Checks on you often"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bSimilarInterests, TEXT("Has similar interests"), Met, Total, Result.Met, Result.Missing);
    AddCheck(Obs.bBuildsInterestsOverTime, TEXT("Builds interests together over time"), Met, Total, Result.Met, Result.Missing);

    Result.ScorePercent = (Total > 0) ? (100.f * float(Met) / float(Total)) : 0.f;
    Result.bPass = (Result.ScorePercent >= RequiredPercent);
    return Result;
}

