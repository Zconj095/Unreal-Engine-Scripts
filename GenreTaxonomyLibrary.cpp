#include "GenreTaxonomyLibrary.h"

// Labels (simple display name echoes)
FString UGenreTaxonomyLibrary::ToLabel_Main(EGenreMain V)
{
    switch (V)
    {
    case EGenreMain::Fantasy:    return TEXT("Fantasy");
    case EGenreMain::Adventure:  return TEXT("Adventure");
    case EGenreMain::Mystical:   return TEXT("Mystical");
    case EGenreMain::Romance:    return TEXT("Romance");
    case EGenreMain::Science:    return TEXT("Science");
    case EGenreMain::Action:     return TEXT("Action");
    case EGenreMain::Magical:    return TEXT("Magical");
    case EGenreMain::Nonfiction: return TEXT("Nonfiction");
    default:                     return TEXT("Fiction");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_SciFic(EGenreSciFicHybrid V)
{
    switch (V)
    {
    case EGenreSciFicHybrid::ScienceFiction:   return TEXT("Science Fiction");
    case EGenreSciFicHybrid::FictionalScience: return TEXT("Fictional Science");
    case EGenreSciFicHybrid::MysticalScience:  return TEXT("Mystical Science");
    case EGenreSciFicHybrid::MagicalScience:   return TEXT("Magical Science");
    case EGenreSciFicHybrid::FringeScience:    return TEXT("Fringe Science");
    default:                                   return TEXT("Fantasy Science");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_AdvAction(EGenreAdvActionHybrid V)
{
    switch (V)
    {
    case EGenreAdvActionHybrid::FantasyAdventure:      return TEXT("Fantasy Adventure");
    case EGenreAdvActionHybrid::MagicalAdventure:      return TEXT("Magical Adventure");
    case EGenreAdvActionHybrid::MysticalAdventure:     return TEXT("Mystical Adventure");
    case EGenreAdvActionHybrid::MagicalAction:         return TEXT("Magical Action");
    case EGenreAdvActionHybrid::FantasyBasedAction:    return TEXT("Fantasy Based Action");
    default:                                           return TEXT("Supernatural Adventure");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_Romance(EGenreRomanceVariant V)
{
    switch (V)
    {
    case EGenreRomanceVariant::MysticalRomance:      return TEXT("Mystical Romance");
    case EGenreRomanceVariant::MagicalRomance:       return TEXT("Magical Romance");
    case EGenreRomanceVariant::ScientificRomance:    return TEXT("Scientific Romance");
    case EGenreRomanceVariant::FringeBasedRomance:   return TEXT("Fringe Based Romance");
    case EGenreRomanceVariant::ReligiousRomance:     return TEXT("Religious Romance");
    case EGenreRomanceVariant::SpiritualRomance:     return TEXT("Spiritual Romance");
    case EGenreRomanceVariant::RomanceAndAction:     return TEXT("Romance & Action");
    case EGenreRomanceVariant::RomanceAndAdventure:  return TEXT("Romance & Adventure");
    case EGenreRomanceVariant::RomanticAdventure:    return TEXT("Romantic Adventure");
    default:                                         return TEXT("Supernatural Romance");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_Ghibli(EGenreGhibliVariant V)
{
    switch (V)
    {
    case EGenreGhibliVariant::GhibliAnime:    return TEXT("Ghibli Anime");
    case EGenreGhibliVariant::GhibliCartoon:  return TEXT("Ghibli Cartoon");
    case EGenreGhibliVariant::GhibliMystical: return TEXT("Ghibli Mystical");
    case EGenreGhibliVariant::GhibliMagical:  return TEXT("Ghibli Magical");
    default:                                  return TEXT("Ghibli Fringe");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_SpiritualReligious(EGenreSpiritualReligious V)
{
    switch (V)
    {
    case EGenreSpiritualReligious::MysticalFringe:      return TEXT("Mystical Fringe");
    case EGenreSpiritualReligious::MagicalFringe:       return TEXT("Magical Fringe");
    case EGenreSpiritualReligious::ReligiousFantasy:    return TEXT("Religious Fantasy");
    case EGenreSpiritualReligious::ReligiousSuspense:   return TEXT("Religious Suspense");
    case EGenreSpiritualReligious::ReligiousAdventure:  return TEXT("Religious Adventure");
    case EGenreSpiritualReligious::ReligiousScience:    return TEXT("Religious Science");
    case EGenreSpiritualReligious::ReligiousFiction:    return TEXT("Religious Fiction");
    case EGenreSpiritualReligious::SpiritualFantasy:    return TEXT("Spiritual Fantasy");
    case EGenreSpiritualReligious::FringeFantasy:       return TEXT("Fringe Fantasy");
    case EGenreSpiritualReligious::SupernaturalFantasy: return TEXT("Supernatural Fantasy");
    default:                                            return TEXT("Nonfiction Fantasy");
    }
}

FString UGenreTaxonomyLibrary::ToLabel_Complex(EGenreComplexHybrid V)
{
    switch (V)
    {
    case EGenreComplexHybrid::ReligiousScienceFictionFantasy: return TEXT("Religious Science Fiction Fantasy");
    case EGenreComplexHybrid::SpiritualMagicalAdventure:      return TEXT("Spiritual Magical Adventure");
    case EGenreComplexHybrid::SupernaturalMagicalAdventure:   return TEXT("Supernatural Magical Adventure");
    case EGenreComplexHybrid::MagicalFantasyRomance:          return TEXT("Magical Fantasy Romance");
    case EGenreComplexHybrid::MagicalFantasyAdventure:        return TEXT("Magical Fantasy Adventure");
    case EGenreComplexHybrid::MagicalRomanticAdventure:       return TEXT("Magical Romantic Adventure");
    case EGenreComplexHybrid::MysticalRomanticAdventure:      return TEXT("Mystical Romantic Adventure");
    case EGenreComplexHybrid::SpiritualFantasyScience:        return TEXT("Spiritual Fantasy Science");
    case EGenreComplexHybrid::MagicalFringeScience:           return TEXT("Magical Fringe Science");
    case EGenreComplexHybrid::MysticalFringeScience:          return TEXT("Mystical Fringe Science");
    case EGenreComplexHybrid::MagicalReligiousFantasy:        return TEXT("Magical Religious Fantasy");
    case EGenreComplexHybrid::ReligiousSpiritualFantasy:      return TEXT("Religious Spiritual Fantasy");
    case EGenreComplexHybrid::SupernaturalFantasyRomance:     return TEXT("Supernatural Fantasy Romance");
    case EGenreComplexHybrid::MagicalFringeRomance:           return TEXT("Magical Fringe Romance");
    case EGenreComplexHybrid::MagicalScientificFantasy:       return TEXT("Magical Scientific Fantasy");
    case EGenreComplexHybrid::MagicalReligiousRomance:        return TEXT("Magical Religious Romance");
    case EGenreComplexHybrid::MagicalReligiousScience:        return TEXT("Magical Religious Science");
    default:                                                  return TEXT("Magical Mystical Religion");
    }
}

// Definitions
FString UGenreTaxonomyLibrary::DefinitionOf_Main(EGenreMain V)
{
    switch (V)
    {
    case EGenreMain::Fantasy:
        return TEXT("A genre of imaginative fiction involving magical elements, mythical creatures, and fantastical worlds, often set in realms where the ordinary laws of nature do not apply.");
    case EGenreMain::Adventure:
        return TEXT("A genre characterized by exciting, often hazardous journeys or quests, typically involving exploration, danger, and a hero facing challenges in unknown or exotic settings.");
    case EGenreMain::Mystical:
        return TEXT("Pertaining to a sense of wonder or mystery, often involving the supernatural, spiritual, or otherworldly experiences that transcend the ordinary understanding of reality.");
    case EGenreMain::Romance:
        return TEXT("A genre centered around love and emotional relationships, focusing on the journey of characters as they navigate the complexities of affection, passion, and connection.");
    case EGenreMain::Science:
        return TEXT("A genre or discipline rooted in the systematic study of the natural world through observation, experimentation, and analysis, often leading to discoveries that explain phenomena or create new technologies.");
    case EGenreMain::Action:
        return TEXT("A genre that emphasizes fast-paced, high-energy sequences, often involving physical feats, combat, and intense situations that keep the audience on the edge of their seat.");
    case EGenreMain::Magical:
        return TEXT("Involving the use of supernatural powers or forces that defy the laws of nature, often found in genres like fantasy, where characters wield abilities that transcend the ordinary.");
    case EGenreMain::Nonfiction:
        return TEXT("A genre based on factual events, people, or information, aiming to inform, educate, or document reality without the embellishments of fiction.");
    case EGenreMain::Fiction:
        return TEXT("A genre encompassing stories created from the imagination, not bound by factual accuracy, allowing for the creation of worlds, characters, and events that may or may not be possible in reality.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_SciFic(EGenreSciFicHybrid V)
{
    switch (V)
    {
    case EGenreSciFicHybrid::ScienceFiction:
        return TEXT("A genre that blends speculative concepts based on science and technology with imaginative narratives, often exploring futuristic scenarios, advanced civilizations, or space exploration.");
    case EGenreSciFicHybrid::FictionalScience:
        return TEXT("Imaginary or speculative scientific concepts used within fictional narratives, often stretching or bending the limits of real-world science to explore new ideas or possibilities.");
    case EGenreSciFicHybrid::MysticalScience:
        return TEXT("A genre that combines scientific concepts with mystical or supernatural elements, exploring the intersection of the empirical and the esoteric.");
    case EGenreSciFicHybrid::MagicalScience:
        return TEXT("A genre where scientific principles are intertwined with magical elements, creating a world where the two coexist or influence each other.");
    case EGenreSciFicHybrid::FringeScience:
        return TEXT("A genre dealing with unconventional or speculative scientific theories and practices, often exploring the outer edges of what is considered possible or accepted by mainstream science.");
    case EGenreSciFicHybrid::FantasyScience:
        return TEXT("A genre that merges the imaginative elements of fantasy with scientific ideas, often creating worlds where magic and science are indistinguishable or work in harmony.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_AdvAction(EGenreAdvActionHybrid V)
{
    switch (V)
    {
    case EGenreAdvActionHybrid::FantasyAdventure:
        return TEXT("A genre that combines the imaginative and mythical elements of fantasy with the excitement and peril of adventure, often featuring quests in fantastical worlds.");
    case EGenreAdvActionHybrid::MagicalAdventure:
        return TEXT("A genre where characters embark on thrilling journeys in a world filled with magic, often facing challenges that require both bravery and magical prowess.");
    case EGenreAdvActionHybrid::MysticalAdventure:
        return TEXT("A genre that blends the wonder and mystery of the mystical with the excitement of an adventure, often involving quests that lead to spiritual or supernatural discoveries.");
    case EGenreAdvActionHybrid::MagicalAction:
        return TEXT("A genre that infuses action-packed sequences with elements of magic, often featuring battles or feats that involve supernatural powers or enchanted artifacts.");
    case EGenreAdvActionHybrid::FantasyBasedAction:
        return TEXT("A genre focusing on intense, physical confrontations and high-stakes scenarios within a fantastical setting, often involving mythical creatures or magical abilities.");
    case EGenreAdvActionHybrid::SupernaturalAdventure:
        return TEXT("A genre where characters embark on adventures that involve supernatural elements, such as ghosts, spirits, or otherworldly beings, often in a quest for understanding or resolution.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_Romance(EGenreRomanceVariant V)
{
    switch (V)
    {
    case EGenreRomanceVariant::MysticalRomance:
        return TEXT("A genre where the romantic narrative is intertwined with mystical or supernatural elements, creating a love story that transcends the ordinary and delves into the unknown.");
    case EGenreRomanceVariant::MagicalRomance:
        return TEXT("A genre where love is woven into a world of magic, with relationships influenced or complicated by magical forces, spells, or enchanted beings.");
    case EGenreRomanceVariant::ScientificRomance:
        return TEXT("A genre that blends elements of romance with science fiction or scientific themes, often exploring love in futuristic or technologically advanced settings.");
    case EGenreRomanceVariant::FringeBasedRomance:
        return TEXT("A genre where romantic narratives are set against a backdrop of fringe science or unconventional theories, creating unique and often speculative romantic dynamics.");
    case EGenreRomanceVariant::ReligiousRomance:
        return TEXT("A genre that explores romantic relationships within the context of religious beliefs, practices, or settings, often dealing with themes of faith, spirituality, and divine intervention.");
    case EGenreRomanceVariant::SpiritualRomance:
        return TEXT("A genre focusing on romantic relationships that are deeply intertwined with spiritual journeys, often exploring the connection between love and spiritual awakening or growth.");
    case EGenreRomanceVariant::RomanceAndAction:
        return TEXT("A genre that combines the emotional depth of romance with the excitement and tension of action, often featuring couples who face physical challenges together.");
    case EGenreRomanceVariant::RomanceAndAdventure:
        return TEXT("A genre that merges the exploration and excitement of adventure with the emotional journey of romance, often featuring couples who bond over shared quests or challenges.");
    case EGenreRomanceVariant::RomanticAdventure:
        return TEXT("A genre focusing on a love story that unfolds in the context of a thrilling adventure, with the characters' romantic connection deepening through their shared experiences.");
    case EGenreRomanceVariant::SupernaturalRomance:
        return TEXT("A genre where love stories are influenced or complicated by supernatural forces, such as ghosts, vampires, or other paranormal entities, often creating intense and otherworldly romantic dynamics.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_Ghibli(EGenreGhibliVariant V)
{
    switch (V)
    {
    case EGenreGhibliVariant::GhibliAnime:
        return TEXT("A genre inspired by Studio Ghibli’s distinctive style, known for its richly imaginative worlds, deep emotional storytelling, and strong environmental and spiritual themes.");
    case EGenreGhibliVariant::GhibliCartoon:
        return TEXT("A genre similar to Ghibli Anime but broader, encompassing animated works that capture the essence of Ghibli’s magic, wonder, and attention to detail, often aimed at a younger audience.");
    case EGenreGhibliVariant::GhibliMystical:
        return TEXT("A genre inspired by the mystical elements found in Ghibli films, combining a sense of wonder, spirituality, and the supernatural within richly animated worlds.");
    case EGenreGhibliVariant::GhibliMagical:
        return TEXT("A genre influenced by the magical themes of Studio Ghibli’s work, featuring enchanting narratives that blend the everyday with the extraordinary in a visually stunning style.");
    case EGenreGhibliVariant::GhibliFringe:
        return TEXT("A genre that draws on Ghibli’s exploration of fringe or unconventional themes, such as obscure mythology, environmentalism, or alternative realities, within a richly animated context.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_SpiritualReligious(EGenreSpiritualReligious V)
{
    switch (V)
    {
    case EGenreSpiritualReligious::MysticalFringe:
        return TEXT("A genre combining the mystical with unconventional or fringe ideas, exploring the boundaries of known reality through a spiritual or otherworldly lens.");
    case EGenreSpiritualReligious::MagicalFringe:
        return TEXT("A genre where magical elements intersect with fringe theories or unconventional practices, often creating narratives that challenge mainstream understanding of magic and reality.");
    case EGenreSpiritualReligious::ReligiousFantasy:
        return TEXT("A genre that blends religious themes, symbols, or narratives with the imaginative and mythical elements of fantasy, often creating epic tales of divine intervention or spiritual quests.");
    case EGenreSpiritualReligious::ReligiousSuspense:
        return TEXT("A genre where religious themes create tension and uncertainty, often involving moral dilemmas, divine mysteries, or spiritual crises that lead to suspenseful narratives.");
    case EGenreSpiritualReligious::ReligiousAdventure:
        return TEXT("A genre that combines religious or spiritual journeys with the excitement of adventure, often involving quests for holy relics, spiritual enlightenment, or divine intervention.");
    case EGenreSpiritualReligious::ReligiousScience:
        return TEXT("A genre exploring the relationship between religious beliefs and scientific concepts, often blending theological ideas with speculative science.");
    case EGenreSpiritualReligious::ReligiousFiction:
        return TEXT("A genre that uses fictional narratives to explore, critique, or illuminate religious beliefs, practices, or experiences, often delving into moral or spiritual questions.");
    case EGenreSpiritualReligious::SpiritualFantasy:
        return TEXT("A genre that merges the elements of fantasy with spiritual themes, often exploring the soul, afterlife, or metaphysical realms through imaginative storytelling.");
    case EGenreSpiritualReligious::FringeFantasy:
        return TEXT("A genre that blends the imaginative worlds of fantasy with fringe or unconventional ideas, creating narratives that challenge traditional boundaries of reality and belief.");
    case EGenreSpiritualReligious::SupernaturalFantasy:
        return TEXT("A genre that combines the elements of fantasy with supernatural occurrences, often featuring worlds where magic, spirits, and otherworldly beings are a natural part of the environment.");
    case EGenreSpiritualReligious::NonfictionFantasy:
        return TEXT("A unique genre that blends factual or real-world elements with fantastical narratives, often using allegory or metaphor to explore real-world issues within an imaginative framework.");
    }
    return FString();
}

FString UGenreTaxonomyLibrary::DefinitionOf_Complex(EGenreComplexHybrid V)
{
    switch (V)
    {
    case EGenreComplexHybrid::ReligiousScienceFictionFantasy:
        return TEXT("A genre that blends the imaginative worlds and speculative technologies of science fiction with the mythical and moral elements of fantasy, all framed within a religious or theological context. This genre explores how religious beliefs and divine interventions interact with advanced technologies and fantastical realms.");
    case EGenreComplexHybrid::SpiritualMagicalAdventure:
        return TEXT("A genre where characters embark on a thrilling journey infused with both spiritual growth and magical elements, often encountering mystical beings or forces that challenge and transform them on a deeper, spiritual level.");
    case EGenreComplexHybrid::SupernaturalMagicalAdventure:
        return TEXT("A genre combining the excitement of adventure with elements of the supernatural and magic, where characters face otherworldly challenges and use magical abilities to overcome obstacles in a world where the boundaries between the natural and the supernatural blur.");
    case EGenreComplexHybrid::MagicalFantasyRomance:
        return TEXT("A genre that intertwines a romantic narrative within a fantastical world filled with magic, where love stories are shaped and influenced by supernatural forces, enchanted creatures, or mystical landscapes.");
    case EGenreComplexHybrid::MagicalFantasyAdventure:
        return TEXT("A genre that combines the imaginative, otherworldly elements of fantasy with the excitement and challenge of an adventure, set in a magical realm where heroes embark on epic quests involving magical powers or artifacts.");
    case EGenreComplexHybrid::MagicalRomanticAdventure:
        return TEXT("A genre that merges the excitement of an adventurous journey with a romantic storyline, all set within a magical world where love and danger are intertwined, and where characters' relationships grow through shared magical experiences.");
    case EGenreComplexHybrid::MysticalRomanticAdventure:
        return TEXT("A genre where a love story unfolds in the midst of a mystical journey, filled with spiritual or supernatural elements that challenge and deepen the romantic bond between characters as they navigate an enchanting and mysterious world.");
    case EGenreComplexHybrid::SpiritualFantasyScience:
        return TEXT("A genre that blends spiritual themes and concepts with elements of fantasy and speculative science, creating narratives where scientific discoveries intersect with spiritual truths in fantastical settings.");
    case EGenreComplexHybrid::MagicalFringeScience:
        return TEXT("A genre combining the wonder of magic with fringe scientific theories, exploring unconventional or speculative scientific ideas through the lens of magical realism or fantasy, often challenging conventional notions of reality.");
    case EGenreComplexHybrid::MysticalFringeScience:
        return TEXT("A genre that merges mystical elements with fringe scientific concepts, exploring the boundaries of known science and spiritual understanding through narratives that blur the lines between the mystical and the empirical.");
    case EGenreComplexHybrid::MagicalReligiousFantasy:
        return TEXT("A genre that intertwines religious themes and narratives with the fantastical elements of magic, creating stories where divine forces and magical powers coexist, often in a world filled with spiritual quests and religious symbolism.");
    case EGenreComplexHybrid::ReligiousSpiritualFantasy:
        return TEXT("A genre that combines religious doctrines and spiritual journeys within a fantastical framework, where characters explore divine mysteries and spiritual truths in a world shaped by both religious and fantastical elements.");
    case EGenreComplexHybrid::SupernaturalFantasyRomance:
        return TEXT("A genre blending supernatural occurrences with the fantastical and romantic, where love stories unfold amidst encounters with ghosts, spirits, or other paranormal entities in a world that defies natural laws.");
    case EGenreComplexHybrid::MagicalFringeRomance:
        return TEXT("A genre that explores romantic relationships within the context of magical and fringe science elements, where love is complicated or enhanced by unconventional scientific theories and mystical forces.");
    case EGenreComplexHybrid::MagicalScientificFantasy:
        return TEXT("A genre that fuses magical elements with scientific concepts within a fantastical setting, exploring how magic and science can coexist or conflict in a world that bends both natural laws and the imagination.");
    case EGenreComplexHybrid::MagicalReligiousRomance:
        return TEXT("A genre where romance is interwoven with both magical and religious themes, creating love stories influenced by divine intervention, religious practices, and magical forces, often set in a world where faith and magic intertwine.");
    case EGenreComplexHybrid::MagicalReligiousScience:
        return TEXT("A genre blending magical elements with religious and scientific themes, exploring how mystical powers, divine beliefs, and scientific discoveries interact in a world where all three are deeply interconnected.");
    case EGenreComplexHybrid::MagicalMysticalReligion:
        return TEXT("A genre that delves into the spiritual and mystical aspects of religion, combining elements of magic with religious practices and beliefs to explore the mysteries of faith, divine power, and the supernatural within a sacred context.");
    }
    return FString();
}

// Lists
TArray<EGenreMain> UGenreTaxonomyLibrary::AllMain()
{
    return { EGenreMain::Fantasy, EGenreMain::Adventure, EGenreMain::Mystical, EGenreMain::Romance, EGenreMain::Science, EGenreMain::Action, EGenreMain::Magical, EGenreMain::Nonfiction, EGenreMain::Fiction };
}

TArray<EGenreSciFicHybrid> UGenreTaxonomyLibrary::AllSciFic()
{
    return { EGenreSciFicHybrid::ScienceFiction, EGenreSciFicHybrid::FictionalScience, EGenreSciFicHybrid::MysticalScience, EGenreSciFicHybrid::MagicalScience, EGenreSciFicHybrid::FringeScience, EGenreSciFicHybrid::FantasyScience };
}

TArray<EGenreAdvActionHybrid> UGenreTaxonomyLibrary::AllAdvAction()
{
    return { EGenreAdvActionHybrid::FantasyAdventure, EGenreAdvActionHybrid::MagicalAdventure, EGenreAdvActionHybrid::MysticalAdventure, EGenreAdvActionHybrid::MagicalAction, EGenreAdvActionHybrid::FantasyBasedAction, EGenreAdvActionHybrid::SupernaturalAdventure };
}

TArray<EGenreRomanceVariant> UGenreTaxonomyLibrary::AllRomance()
{
    return { EGenreRomanceVariant::MysticalRomance, EGenreRomanceVariant::MagicalRomance, EGenreRomanceVariant::ScientificRomance, EGenreRomanceVariant::FringeBasedRomance, EGenreRomanceVariant::ReligiousRomance, EGenreRomanceVariant::SpiritualRomance, EGenreRomanceVariant::RomanceAndAction, EGenreRomanceVariant::RomanceAndAdventure, EGenreRomanceVariant::RomanticAdventure, EGenreRomanceVariant::SupernaturalRomance };
}

TArray<EGenreGhibliVariant> UGenreTaxonomyLibrary::AllGhibli()
{
    return { EGenreGhibliVariant::GhibliAnime, EGenreGhibliVariant::GhibliCartoon, EGenreGhibliVariant::GhibliMystical, EGenreGhibliVariant::GhibliMagical, EGenreGhibliVariant::GhibliFringe };
}

TArray<EGenreSpiritualReligious> UGenreTaxonomyLibrary::AllSpiritualReligious()
{
    return { EGenreSpiritualReligious::MysticalFringe, EGenreSpiritualReligious::MagicalFringe, EGenreSpiritualReligious::ReligiousFantasy, EGenreSpiritualReligious::ReligiousSuspense, EGenreSpiritualReligious::ReligiousAdventure, EGenreSpiritualReligious::ReligiousScience, EGenreSpiritualReligious::ReligiousFiction, EGenreSpiritualReligious::SpiritualFantasy, EGenreSpiritualReligious::FringeFantasy, EGenreSpiritualReligious::SupernaturalFantasy, EGenreSpiritualReligious::NonfictionFantasy };
}

TArray<EGenreComplexHybrid> UGenreTaxonomyLibrary::AllComplex()
{
    return { EGenreComplexHybrid::ReligiousScienceFictionFantasy, EGenreComplexHybrid::SpiritualMagicalAdventure, EGenreComplexHybrid::SupernaturalMagicalAdventure, EGenreComplexHybrid::MagicalFantasyRomance, EGenreComplexHybrid::MagicalFantasyAdventure, EGenreComplexHybrid::MagicalRomanticAdventure, EGenreComplexHybrid::MysticalRomanticAdventure, EGenreComplexHybrid::SpiritualFantasyScience, EGenreComplexHybrid::MagicalFringeScience, EGenreComplexHybrid::MysticalFringeScience, EGenreComplexHybrid::MagicalReligiousFantasy, EGenreComplexHybrid::ReligiousSpiritualFantasy, EGenreComplexHybrid::SupernaturalFantasyRomance, EGenreComplexHybrid::MagicalFringeRomance, EGenreComplexHybrid::MagicalScientificFantasy, EGenreComplexHybrid::MagicalReligiousRomance, EGenreComplexHybrid::MagicalReligiousScience, EGenreComplexHybrid::MagicalMysticalReligion };
}

