#include "HumanCharacteristicsPokemonLibrary.h"

FString UHumanCharacteristicsPokemonLibrary::ToLabel(EPokemonInspiredCharacteristic Charac)
{
    switch (Charac)
    {
    case EPokemonInspiredCharacteristic::AlertToSounds:       return TEXT("Alert to Sounds");
    case EPokemonInspiredCharacteristic::GoodEndurance:       return TEXT("Good Endurance");
    case EPokemonInspiredCharacteristic::GoodPerseverance:    return TEXT("Good Perseverance");
    case EPokemonInspiredCharacteristic::HighlyCurious:       return TEXT("Highly Curious");
    case EPokemonInspiredCharacteristic::HighlyPersistent:    return TEXT("Highly Persistent");
    case EPokemonInspiredCharacteristic::ImpetuousAndSilly:   return TEXT("Impetuous and Silly");
    case EPokemonInspiredCharacteristic::LikesToRelax:        return TEXT("Likes to Relax");
    case EPokemonInspiredCharacteristic::LikesToRun:          return TEXT("Likes to Run");
    case EPokemonInspiredCharacteristic::LovesToEat:          return TEXT("Loves to Eat");
    case EPokemonInspiredCharacteristic::Mischievous:         return TEXT("Mischievous");
    case EPokemonInspiredCharacteristic::NodsOffALot:         return TEXT("Nods Off a Lot");
    case EPokemonInspiredCharacteristic::OftenLostInThought:  return TEXT("Often Lost in Thought");
    case EPokemonInspiredCharacteristic::ProudOfItsPower:     return TEXT("Proud of Its Power");
    case EPokemonInspiredCharacteristic::QuickTempered:       return TEXT("Quick Tempered");
    case EPokemonInspiredCharacteristic::QuickToFlee:         return TEXT("Quick to Flee");
    case EPokemonInspiredCharacteristic::ScattersThingsOften: return TEXT("Scatters Things Often");
    case EPokemonInspiredCharacteristic::SomewhatOfAClown:    return TEXT("Somewhat of a Clown");
    case EPokemonInspiredCharacteristic::SomewhatStubborn:    return TEXT("Somewhat Stubborn");
    case EPokemonInspiredCharacteristic::SomewhatVain:        return TEXT("Somewhat Vain");
    case EPokemonInspiredCharacteristic::StrongWilled:        return TEXT("Strong Willed");
    case EPokemonInspiredCharacteristic::StronglyDefiant:     return TEXT("Strongly Defiant");
    case EPokemonInspiredCharacteristic::SturdyBody:          return TEXT("Sturdy Body");
    case EPokemonInspiredCharacteristic::TakesPlentyOfBreaks: return TEXT("Takes Plenty of Breaks");
    case EPokemonInspiredCharacteristic::ThoroughlyCunning:   return TEXT("Thoroughly Cunning");
    default:                                                  return TEXT("Very Finicky");
    }
}

FString UHumanCharacteristicsPokemonLibrary::DefinitionOf(EPokemonInspiredCharacteristic Charac)
{
    switch (Charac)
    {
    case EPokemonInspiredCharacteristic::AlertToSounds:
        return TEXT("Highly responsive to auditory stimuli, quickly noticing and reacting to noises.");
    case EPokemonInspiredCharacteristic::GoodEndurance:
        return TEXT("Capable of sustaining prolonged physical or mental effort, able to perform tasks over extended periods without tiring easily.");
    case EPokemonInspiredCharacteristic::GoodPerseverance:
        return TEXT("Demonstrates determination and persistence, not easily discouraged by challenges, and continues working toward goals despite obstacles.");
    case EPokemonInspiredCharacteristic::HighlyCurious:
        return TEXT("Shows a strong desire to learn and explore new things, often inquisitive about their surroundings and eager to discover more.");
    case EPokemonInspiredCharacteristic::HighlyPersistent:
        return TEXT("Tenacious in pursuit of goals, not giving up easily, and continually striving to achieve objectives even in the face of difficulty.");
    case EPokemonInspiredCharacteristic::ImpetuousAndSilly:
        return TEXT("Tends to act on impulse without much thought, often displaying playful or whimsical behavior.");
    case EPokemonInspiredCharacteristic::LikesToRelax:
        return TEXT("Prefers taking it easy and enjoying leisure time, often seeking out opportunities to rest and unwind.");
    case EPokemonInspiredCharacteristic::LikesToRun:
        return TEXT("Enjoys physical activity, particularly running, and often seeks opportunities to move quickly.");
    case EPokemonInspiredCharacteristic::LovesToEat:
        return TEXT("Has a strong appetite or a great fondness for food, often eager to indulge in meals or snacks.");
    case EPokemonInspiredCharacteristic::Mischievous:
        return TEXT("Enjoys causing playful trouble or engaging in naughty behavior, often in a playful or humorous manner.");
    case EPokemonInspiredCharacteristic::NodsOffALot:
        return TEXT("Frequently falls asleep or dozes off, indicating a tendency to be sleepy or relaxed.");
    case EPokemonInspiredCharacteristic::OftenLostInThought:
        return TEXT("Frequently absorbed in deep or daydreaming thoughts, sometimes appearing distracted or introspective.");
    case EPokemonInspiredCharacteristic::ProudOfItsPower:
        return TEXT("Takes pride in its strength or abilities, often confident and self-assured in its capabilities.");
    case EPokemonInspiredCharacteristic::QuickTempered:
        return TEXT("Easily angered or irritated, often reacting strongly and swiftly to perceived provocations.");
    case EPokemonInspiredCharacteristic::QuickToFlee:
        return TEXT("Tends to avoid confrontation or danger, often escaping or running away at the first sign of trouble.");
    case EPokemonInspiredCharacteristic::ScattersThingsOften:
        return TEXT("Frequently disorganized or prone to making a mess, often leaving things strewn about.");
    case EPokemonInspiredCharacteristic::SomewhatOfAClown:
        return TEXT("Exhibits playful, humorous behavior, often enjoying entertaining others with silly antics.");
    case EPokemonInspiredCharacteristic::SomewhatStubborn:
        return TEXT("Resistant to change or direction, often insisting on doing things their own way despite advice or pressure.");
    case EPokemonInspiredCharacteristic::SomewhatVain:
        return TEXT("Shows a tendency to be overly concerned with appearance or self-image, often paying extra attention to how they are perceived by others.");
    case EPokemonInspiredCharacteristic::StrongWilled:
        return TEXT("Possesses a firm resolve and determination, often sticking to decisions and actions with conviction.");
    case EPokemonInspiredCharacteristic::StronglyDefiant:
        return TEXT("Tends to resist authority or rules, often challenging norms and refusing to conform to expectations.");
    case EPokemonInspiredCharacteristic::SturdyBody:
        return TEXT("Physically robust and resilient, capable of withstanding physical challenges or harsh conditions.");
    case EPokemonInspiredCharacteristic::TakesPlentyOfBreaks:
        return TEXT("Prefers to rest frequently, often pausing during tasks to relax and recharge.");
    case EPokemonInspiredCharacteristic::ThoroughlyCunning:
        return TEXT("Displays cleverness and shrewdness, often using intelligence and strategy to achieve goals.");
    case EPokemonInspiredCharacteristic::VeryFinicky:
        return TEXT("Extremely particular or difficult to please, often requiring things to be done in a specific way or to a high standard of precision.");
    }
    return FString();
}

TArray<EPokemonInspiredCharacteristic> UHumanCharacteristicsPokemonLibrary::AllCharacteristics()
{
    return {
        EPokemonInspiredCharacteristic::AlertToSounds,
        EPokemonInspiredCharacteristic::GoodEndurance,
        EPokemonInspiredCharacteristic::GoodPerseverance,
        EPokemonInspiredCharacteristic::HighlyCurious,
        EPokemonInspiredCharacteristic::HighlyPersistent,
        EPokemonInspiredCharacteristic::ImpetuousAndSilly,
        EPokemonInspiredCharacteristic::LikesToRelax,
        EPokemonInspiredCharacteristic::LikesToRun,
        EPokemonInspiredCharacteristic::LovesToEat,
        EPokemonInspiredCharacteristic::Mischievous,
        EPokemonInspiredCharacteristic::NodsOffALot,
        EPokemonInspiredCharacteristic::OftenLostInThought,
        EPokemonInspiredCharacteristic::ProudOfItsPower,
        EPokemonInspiredCharacteristic::QuickTempered,
        EPokemonInspiredCharacteristic::QuickToFlee,
        EPokemonInspiredCharacteristic::ScattersThingsOften,
        EPokemonInspiredCharacteristic::SomewhatOfAClown,
        EPokemonInspiredCharacteristic::SomewhatStubborn,
        EPokemonInspiredCharacteristic::SomewhatVain,
        EPokemonInspiredCharacteristic::StrongWilled,
        EPokemonInspiredCharacteristic::StronglyDefiant,
        EPokemonInspiredCharacteristic::SturdyBody,
        EPokemonInspiredCharacteristic::TakesPlentyOfBreaks,
        EPokemonInspiredCharacteristic::ThoroughlyCunning,
        EPokemonInspiredCharacteristic::VeryFinicky
    };
}

