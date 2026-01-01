#include "CustomFeelingsLibrary.h"

FString UCustomFeelingsLibrary::ToLabel(ECustomFeeling Feeling)
{
    switch (Feeling)
    {
    case ECustomFeeling::Acceptance:      return TEXT("Acceptance");
    case ECustomFeeling::Admiration:      return TEXT("Admiration");
    case ECustomFeeling::Affectionate:    return TEXT("Affectionate");
    case ECustomFeeling::Altruistic:      return TEXT("Altruistic");
    case ECustomFeeling::Amused:          return TEXT("Amused");
    case ECustomFeeling::Beautiful:       return TEXT("Beautiful");
    case ECustomFeeling::Blessed:         return TEXT("Blessed");
    case ECustomFeeling::Brave:           return TEXT("Brave");
    case ECustomFeeling::Calm:            return TEXT("Calm");
    case ECustomFeeling::Cheerful:        return TEXT("Cheerful");
    case ECustomFeeling::Collected:       return TEXT("Collected");
    case ECustomFeeling::Concerned:       return TEXT("Concerned");
    case ECustomFeeling::Confident:       return TEXT("Confident");
    case ECustomFeeling::Content:         return TEXT("Content");
    case ECustomFeeling::Courageous:      return TEXT("Courageous");
    case ECustomFeeling::Defensive:       return TEXT("Defensive");
    case ECustomFeeling::Determined:      return TEXT("Determined");
    case ECustomFeeling::Eager:           return TEXT("Eager");
    case ECustomFeeling::Emotional:       return TEXT("Emotional");
    case ECustomFeeling::Empathic:        return TEXT("Empathic");
    case ECustomFeeling::Energetic:       return TEXT("Energetic");
    case ECustomFeeling::Faith:           return TEXT("Faith");
    case ECustomFeeling::Faithful:        return TEXT("Faithful");
    case ECustomFeeling::Fascinated:      return TEXT("Fascinated");
    case ECustomFeeling::Focused:         return TEXT("Focused");
    case ECustomFeeling::Full_Of_Purpose: return TEXT("Full Of Purpose");
    case ECustomFeeling::Good:            return TEXT("Good");
    case ECustomFeeling::Graceful:        return TEXT("Graceful");
    case ECustomFeeling::Grateful:        return TEXT("Grateful");
    case ECustomFeeling::Great:           return TEXT("Great");
    case ECustomFeeling::Happy:           return TEXT("Happy");
    case ECustomFeeling::Honor:           return TEXT("Honor");
    case ECustomFeeling::Honorable:       return TEXT("Honorable");
    case ECustomFeeling::Hopeful:         return TEXT("Hopeful");
    case ECustomFeeling::Impressed:       return TEXT("Impressed");
    case ECustomFeeling::Independent:     return TEXT("Independent");
    case ECustomFeeling::Liked:           return TEXT("Liked");
    case ECustomFeeling::Loved:           return TEXT("Loved");
    case ECustomFeeling::Motivated:       return TEXT("Motivated");
    case ECustomFeeling::Neutral:         return TEXT("Neutral");
    case ECustomFeeling::Offensive:       return TEXT("Offensive");
    case ECustomFeeling::Optimistic:      return TEXT("Optimistic");
    case ECustomFeeling::Positive:        return TEXT("Positive");
    case ECustomFeeling::Powerful:        return TEXT("Powerful");
    case ECustomFeeling::Proud:           return TEXT("Proud");
    case ECustomFeeling::Relaxed:         return TEXT("Relaxed");
    case ECustomFeeling::Relief:          return TEXT("Relief");
    case ECustomFeeling::Selfless:        return TEXT("Selfless");
    case ECustomFeeling::Sensational:     return TEXT("Sensational");
    case ECustomFeeling::Sensitive:       return TEXT("Sensitive");
    case ECustomFeeling::Social:          return TEXT("Social");
    case ECustomFeeling::Special:         return TEXT("Special");
    case ECustomFeeling::Strong:          return TEXT("Strong");
    case ECustomFeeling::Surprised:       return TEXT("Surprised");
    case ECustomFeeling::Sympathetic:     return TEXT("Sympathetic");
    case ECustomFeeling::Thankful:        return TEXT("Thankful");
    case ECustomFeeling::Thoughtful:      return TEXT("Thoughtful");
    case ECustomFeeling::Thrilled:        return TEXT("Thrilled");
    default:                              return TEXT("Unique");
    }
}

FString UCustomFeelingsLibrary::DefinitionOf(ECustomFeeling Feeling)
{
    switch (Feeling)
    {
    case ECustomFeeling::Acceptance:
        return TEXT("A Feeling That Describes The Capability To Accept Some Kind Of Circumstance And Or Action And Or Behavior");
    case ECustomFeeling::Admiration:
        return TEXT("A Feeling That Describes A Sense Of Respect Or Honor To Some Form Of Action And Or Cause And Or Effect");
    case ECustomFeeling::Affectionate:
        return TEXT("A Feeling That Describes The Feel Of Affection And How Intense And Or Strong It Is");
    case ECustomFeeling::Altruistic:
        return TEXT("A Feeling That Describes The Connection Of Strong Beliefs And Truth Values Brought Together");
    case ECustomFeeling::Amused:
        return TEXT("A Feeling That Describes A Sense Of Enjoyment Formed From Experiencing Something Or Visually Seeing Something With Either Sight Or Vision");
    case ECustomFeeling::Beautiful:
        return TEXT("A Feeling Of Commitment Of How Something Looks And The Feeling Is Felt When Experiencing The Quality Of The Feeling Of The Quality Of Beauty");
    case ECustomFeeling::Blessed:
        return TEXT("A Feeling Of A Strong Emotion Of Thankfulness And Gratefulness Formed From The Emotions And Moods Of Grateful And Thankful Emotions");
    case ECustomFeeling::Brave:
        return TEXT("A Feeling That Describes Actions That Involve Risk Or Dangerous Events Or Survival With Courage And Bravery To Enter The Circumstances While Challenging Fear With Pure Feeling");
    case ECustomFeeling::Calm:
        return TEXT("A Feeling That Describes Feelings Forming Inner Peace Within A Sense Of Calamity Or Chaos");
    case ECustomFeeling::Cheerful:
        return TEXT("A Feeling That Describes Feelings Full Of Cheer");
    case ECustomFeeling::Collected:
        return TEXT("A Feeling Described As Calm And Focused And Content Simultaneously");
    case ECustomFeeling::Concerned:
        return TEXT("A Feeling That Shows A Sense Of Worry With Content Emotions Pushing Through The Border Or Worry And Feeling");
    case ECustomFeeling::Confident:
        return TEXT("A Feeling Described As Having A Strong Feel That The Answer Is Either Known Already Or Near To Becoming Solved");
    case ECustomFeeling::Content:
        return TEXT("A Feeling Of Calm And Relaxed Behavior");
    case ECustomFeeling::Courageous:
        return TEXT("A Feeling Described As Full Of Courage");
    case ECustomFeeling::Defensive:
        return TEXT("A Feeling That Can Be Described As Feelings That Are Defending Something And Or Someone For Some Form Of Reason And Or Belief");
    case ECustomFeeling::Determined:
        return TEXT("A Feeling So Strong That Is Described Action And Feeling Together When Forming Motivated Changes Internally Or Externally To See Success Form And Or For Something To Happen");
    case ECustomFeeling::Eager:
        return TEXT("A Feeling Of Desire For Something To Happen That Is Upcoming Or Very Close To Happening");
    case ECustomFeeling::Emotional:
        return TEXT("A Feeling Describing Many Emotions Coming Together Within The Chakras And Or Auras And Or Electromagnetic Field");
    case ECustomFeeling::Empathic:
        return TEXT("A Feeling Describing The Understanding Of Others Emotions More Than Knowing Ones Own Emotions Internally");
    case ECustomFeeling::Energetic:
        return TEXT("A Feeling Described As A Felt Emotion And Or Mood And Or Feeling That Continuously Is Filled With Energy");
    case ECustomFeeling::Faith:
        return TEXT("A Feeling Describing The Connections Between Beliefs And Emotions As Well As Moods And Feelings While Holding Strong Desires Internally");
    case ECustomFeeling::Faithful:
        return TEXT("A Feeling Describing Commitment And Loyalty To A Specific Action And Or Cause And Or Circumstance And Or Person");
    case ECustomFeeling::Fascinated:
        return TEXT("A Feeling Described As Full Of Enjoyment And Satisfaction To Experiences");
    case ECustomFeeling::Focused:
        return TEXT("A Feeling Described As Feeling A Specific Emotion Towards Something Specific With Intensity");
    case ECustomFeeling::Full_Of_Purpose:
        return TEXT("A Feeling Described As Having A Large Amount Of Meaning And Or Definitions Given A Purpose And Meaning");
    case ECustomFeeling::Good:
        return TEXT("A Feeling Described As Feeling Satisfaction And Positive Feelings");
    case ECustomFeeling::Graceful:
        return TEXT("A Feeling Described As Feeling Grace And Fluid Movement Swiftly And Vividly");
    case ECustomFeeling::Grateful:
        return TEXT("A Feeling Described As Having Thankful And Satisfaction From Blessed Circumstances");
    case ECustomFeeling::Great:
        return TEXT("A Feeling Described As Feeling Positive And Good About The Circumstances With Confident Beliefs And Intensity Of Emotions And Moods With The Feelings");
    case ECustomFeeling::Happy:
        return TEXT("A Feeling Described As Feeling Good And Satisfied From The Circumstances And Events That Formed The Feeling Itself");
    case ECustomFeeling::Honor:
        return TEXT("A Feeling Described As Following Through To Ones Commitments From A Higher Being Or Entity That Brings A Sense Of Satisfaction For Living For Someone Else Instead Of Oneself");
    case ECustomFeeling::Honorable:
        return TEXT("A Feeling Described As Good To The Commitments Made And The Promises Made");
    case ECustomFeeling::Hopeful:
        return TEXT("A Feeling Described As Having Hope That Something Can Happen By Beliefs And Willpower Only");
    case ECustomFeeling::Impressed:
        return TEXT("A Feeling Described As Having A Satisfied Reaction From An Action Or Effect");
    case ECustomFeeling::Independent:
        return TEXT("A Feeling Described As Having A Strong Feeling Of Independence Made From Feelings Rather Than Moods And Emotional Behaviors");
    case ECustomFeeling::Liked:
        return TEXT("A Feeling Described As Having Acceptance Towards Someone Else's Emotions And Or Moods And Or Feelings");
    case ECustomFeeling::Loved:
        return TEXT("A Feeling When Love Is Happening With A Form Of Safety And Comfort And The Emotion Serenity While Feeling Content With A Relaxed Mood");
    case ECustomFeeling::Motivated:
        return TEXT("To Be Filled With Passion And Desire And Determination To Achieve Something Such As A Goal Or Task Or Series Of Tasks And Or Goals");
    case ECustomFeeling::Neutral:
        return TEXT("To Not Be Moved By Negative Or Positive Circumstances Or Actions");
    case ECustomFeeling::Offensive:
        return TEXT("A Feeling That Defines A Forming Action With Feelings And Emotions");
    case ECustomFeeling::Optimistic:
        return TEXT("A Feeling That Describes Making Actions Based On Understanding That There Is Hope And Or Faith That What Is Determined To Be Achieving With Confidence Can And Should Happen By Mindset");
    case ECustomFeeling::Positive:
        return TEXT("A Feeling That Describes The Ability To See And Or Envision The Higher Values That Can Be Reached And For It Without Becoming Affected By Negative Feelings");
    case ECustomFeeling::Powerful:
        return TEXT("A Feeling Described By Containing A Large Power And Considered Capable And Someone That Can Manage Self Defined Actions With That Power");
    case ECustomFeeling::Proud:
        return TEXT("A Feeling Of Becoming Happy And Forming Satisfaction Of What Has Been Able To Succeed With Or Is Succeeding To Accomplish And Or Achieve Something");
    case ECustomFeeling::Relaxed:
        return TEXT("A Feeling Of Contentment And Feeling Of Enjoyment Combined Together");
    case ECustomFeeling::Relief:
        return TEXT("A Sense Of Thankful Feeling As A Sense Of Grateful Feeling Combine Together");
    case ECustomFeeling::Selfless:
        return TEXT("A Feeling Defined As Becoming Able To Focus Towards Others Other Than Of Oneself");
    case ECustomFeeling::Sensational:
        return TEXT("A Sense Of Enjoyment And Experience That Forms From The Haptic_Feedback Responses And Sensations And Experiences Felt");
    case ECustomFeeling::Sensitive:
        return TEXT("A Feeling Of High Sensitivity And Or Intensity");
    case ECustomFeeling::Social:
        return TEXT("A Feeling Of Wanting To Form Communication");
    case ECustomFeeling::Special:
        return TEXT("A Feeling Of Unique And Individual And Abstract Simultaneously");
    case ECustomFeeling::Strong:
        return TEXT("Having A Sense Of Feeling That Forms With Determination And Willpower While Not Failing To Allow Becoming Lesser To Form And To Stay Within The Position Of Diligence And Strength");
    case ECustomFeeling::Surprised:
        return TEXT("A Feeling Of Succeeding Expectations Of What Was Expected To Happen");
    case ECustomFeeling::Sympathetic:
        return TEXT("A Feeling Of Understanding Someone Through Empathic Thoughts And Or Feelings And Or Sensations");
    case ECustomFeeling::Thankful:
        return TEXT("A Feeling Describing Relief And Gratitude Together");
    case ECustomFeeling::Thoughtful:
        return TEXT("A Feeling Describing Positive Actions Created By Thought");
    case ECustomFeeling::Thrilled:
        return TEXT("A Feeling Describing Immense Satisfaction");
    case ECustomFeeling::Unique:
        return TEXT("A Feeling Describing Something Unique And Special");
    }
    return FString();
}

TArray<ECustomFeeling> UCustomFeelingsLibrary::AllFeelings()
{
    TArray<ECustomFeeling> Out;
    Out.Reserve(59);
    Out.Add(ECustomFeeling::Acceptance);
    Out.Add(ECustomFeeling::Admiration);
    Out.Add(ECustomFeeling::Affectionate);
    Out.Add(ECustomFeeling::Altruistic);
    Out.Add(ECustomFeeling::Amused);
    Out.Add(ECustomFeeling::Beautiful);
    Out.Add(ECustomFeeling::Blessed);
    Out.Add(ECustomFeeling::Brave);
    Out.Add(ECustomFeeling::Calm);
    Out.Add(ECustomFeeling::Cheerful);
    Out.Add(ECustomFeeling::Collected);
    Out.Add(ECustomFeeling::Concerned);
    Out.Add(ECustomFeeling::Confident);
    Out.Add(ECustomFeeling::Content);
    Out.Add(ECustomFeeling::Courageous);
    Out.Add(ECustomFeeling::Defensive);
    Out.Add(ECustomFeeling::Determined);
    Out.Add(ECustomFeeling::Eager);
    Out.Add(ECustomFeeling::Emotional);
    Out.Add(ECustomFeeling::Empathic);
    Out.Add(ECustomFeeling::Energetic);
    Out.Add(ECustomFeeling::Faith);
    Out.Add(ECustomFeeling::Faithful);
    Out.Add(ECustomFeeling::Fascinated);
    Out.Add(ECustomFeeling::Focused);
    Out.Add(ECustomFeeling::Full_Of_Purpose);
    Out.Add(ECustomFeeling::Good);
    Out.Add(ECustomFeeling::Graceful);
    Out.Add(ECustomFeeling::Grateful);
    Out.Add(ECustomFeeling::Great);
    Out.Add(ECustomFeeling::Happy);
    Out.Add(ECustomFeeling::Honor);
    Out.Add(ECustomFeeling::Honorable);
    Out.Add(ECustomFeeling::Hopeful);
    Out.Add(ECustomFeeling::Impressed);
    Out.Add(ECustomFeeling::Independent);
    Out.Add(ECustomFeeling::Liked);
    Out.Add(ECustomFeeling::Loved);
    Out.Add(ECustomFeeling::Motivated);
    Out.Add(ECustomFeeling::Neutral);
    Out.Add(ECustomFeeling::Offensive);
    Out.Add(ECustomFeeling::Optimistic);
    Out.Add(ECustomFeeling::Positive);
    Out.Add(ECustomFeeling::Powerful);
    Out.Add(ECustomFeeling::Proud);
    Out.Add(ECustomFeeling::Relaxed);
    Out.Add(ECustomFeeling::Relief);
    Out.Add(ECustomFeeling::Selfless);
    Out.Add(ECustomFeeling::Sensational);
    Out.Add(ECustomFeeling::Sensitive);
    Out.Add(ECustomFeeling::Social);
    Out.Add(ECustomFeeling::Special);
    Out.Add(ECustomFeeling::Strong);
    Out.Add(ECustomFeeling::Surprised);
    Out.Add(ECustomFeeling::Sympathetic);
    Out.Add(ECustomFeeling::Thankful);
    Out.Add(ECustomFeeling::Thoughtful);
    Out.Add(ECustomFeeling::Thrilled);
    Out.Add(ECustomFeeling::Unique);
    return Out;
}

