#include "CoreSciencesLibrary.h"

// Labels
FString UCoreSciencesLibrary::ToLabel_Core(ECoreScienceTech V)
{
    switch (V)
    {
    case ECoreScienceTech::QuantumScience:        return TEXT("Quantum Science");
    case ECoreScienceTech::ComputerScience:       return TEXT("Computer Science");
    case ECoreScienceTech::Hardwares:             return TEXT("Hardwares");
    case ECoreScienceTech::Softwares:             return TEXT("Softwares");
    case ECoreScienceTech::Bioevolution:          return TEXT("Bioevolution");
    case ECoreScienceTech::Biological:            return TEXT("Biological");
    case ECoreScienceTech::Biochemistry:          return TEXT("Biochemistry");
    case ECoreScienceTech::Geochemistry:          return TEXT("Geochemistry");
    case ECoreScienceTech::Crystallography:       return TEXT("Crystallography");
    case ECoreScienceTech::Chrononautology:       return TEXT("Chrononautology");
    case ECoreScienceTech::Atomization:           return TEXT("Atomization");
    case ECoreScienceTech::ChronoBiology:         return TEXT("Chrono Biology");
    case ECoreScienceTech::Genetics:              return TEXT("Genetics");
    case ECoreScienceTech::Oceanography:          return TEXT("Oceanography");
    case ECoreScienceTech::Ecology:               return TEXT("Ecology");
    case ECoreScienceTech::EnvironmentalScience:  return TEXT("Environmental Science");
    case ECoreScienceTech::ScientificEngineering: return TEXT("Scientific Engineering");
    case ECoreScienceTech::ScientificTechnician:  return TEXT("Scientific Technician");
    case ECoreScienceTech::LunarSciences:         return TEXT("Lunar Sciences");
    case ECoreScienceTech::SolarSciences:         return TEXT("Solar Sciences");
    case ECoreScienceTech::SeasonalSciences:      return TEXT("Seasonal Sciences");
    case ECoreScienceTech::CelestialSciences:     return TEXT("Celestial Sciences");
    case ECoreScienceTech::Energetics:            return TEXT("Energetics");
    case ECoreScienceTech::ParticlePhysics:       return TEXT("Particle Physics");
    case ECoreScienceTech::Psychology:            return TEXT("Psychology");
    case ECoreScienceTech::PsychologyOfPlay:      return TEXT("Psychology of Play");
    case ECoreScienceTech::Inventions:            return TEXT("Inventions");
    case ECoreScienceTech::ScientificDesigns:     return TEXT("Scientific Designs");
    case ECoreScienceTech::Meteorology:           return TEXT("Meteorology");
    case ECoreScienceTech::Astrology:             return TEXT("Astrology");
    default:                                      return TEXT("Game Engine Development");
    }
}

FString UCoreSciencesLibrary::ToLabel_Advanced(EAdvancedBioNeuro V)
{
    switch (V)
    {
    case EAdvancedBioNeuro::Neuroevolution:     return TEXT("Neuroevolution");
    case EAdvancedBioNeuro::Neuroscience:       return TEXT("Neuroscience");
    case EAdvancedBioNeuro::Neuroanatomy:       return TEXT("Neuroanatomy");
    case EAdvancedBioNeuro::Biosynthesis:       return TEXT("Biosynthesis");
    case EAdvancedBioNeuro::Bioenergetics:      return TEXT("Bioenergetics");
    case EAdvancedBioNeuro::Biosynergetics:     return TEXT("Biosynergetics");
    case EAdvancedBioNeuro::BiomechanicalFusion:return TEXT("Biomechanical Fusion");
    default:                                    return TEXT("Cybernetics");
    }
}

FString UCoreSciencesLibrary::ToLabel_EnergyPhys(EEnergyPhysicsEngineering V)
{
    switch (V)
    {
    case EEnergyPhysicsEngineering::FusionScience:           return TEXT("Fusion Science");
    case EEnergyPhysicsEngineering::NuclearScience:          return TEXT("Nuclear Science");
    case EEnergyPhysicsEngineering::FissionAndFusionScience: return TEXT("Fission and Fusion Science");
    case EEnergyPhysicsEngineering::Thermodynamics:          return TEXT("Thermodynamics");
    case EEnergyPhysicsEngineering::Aerodynamics:            return TEXT("Aerodynamics");
    case EEnergyPhysicsEngineering::ScienceOfFrequency:      return TEXT("The Science of Frequency");
    case EEnergyPhysicsEngineering::ScienceOfSequential:     return TEXT("The Science of Sequential");
    case EEnergyPhysicsEngineering::ScienceOfProtons:        return TEXT("The Science of Protons");
    case EEnergyPhysicsEngineering::ScienceOfElectrons:      return TEXT("The Science of Electrons");
    case EEnergyPhysicsEngineering::ScienceOfGravity:        return TEXT("The Science of Gravity");
    case EEnergyPhysicsEngineering::ScienceOfAI:             return TEXT("The Science of Artificial Intelligence");
    case EEnergyPhysicsEngineering::ScienceOfGenes:          return TEXT("The Science of Genes");
    default:                                                 return TEXT("The Science of Inheritance of Inherited Traits, Skills, Abilities, and Arts");
    }
}

// Definitions
FString UCoreSciencesLibrary::DefinitionOf_Core(ECoreScienceTech V)
{
    switch (V)
    {
    case ECoreScienceTech::QuantumScience:
        return TEXT("The study of the smallest particles and their interactions, governed by the principles of quantum mechanics, which explores the behavior of matter and energy at the atomic and subatomic levels.");
    case ECoreScienceTech::ComputerScience:
        return TEXT("The study of computation, algorithms, and information processing, including the design of computer systems, software development, artificial intelligence, and data analysis.");
    case ECoreScienceTech::Hardwares:
        return TEXT("The physical components of computer systems and electronic devices, including processors, memory, storage, and peripherals that enable the functionality of software.");
    case ECoreScienceTech::Softwares:
        return TEXT("The programs and operating systems that run on computer hardware, enabling users to perform specific tasks, from basic functions to complex applications and services.");
    case ECoreScienceTech::Bioevolution:
        return TEXT("The study of the evolutionary processes that have led to the diversity of life on Earth, focusing on the genetic changes in populations over time and the development of new species.");
    case ECoreScienceTech::Biological:
        return TEXT("Pertaining to the science of life and living organisms, encompassing fields such as genetics, ecology, physiology, and molecular biology.");
    case ECoreScienceTech::Biochemistry:
        return TEXT("The study of the chemical processes and substances that occur within living organisms, exploring the molecular mechanisms that underlie biological functions and structures.");
    case ECoreScienceTech::Geochemistry:
        return TEXT("The study of the chemical composition of the Earth and other planets, including the processes that govern the distribution and cycling of chemical elements in geological systems.");
    case ECoreScienceTech::Crystallography:
        return TEXT("The study of the arrangement of atoms in crystalline solids, as well as many different properties, shapes and attributes that go along with the classification and typing of crystal matter and antimatter as well as its interaction between magnetism and ferromagnetism ,using techniques to understand the structure and properties of crystals.");
    case ECoreScienceTech::Chrononautology:
        return TEXT("A speculative or theoretical science focusing on the study and exploration of time, including the mechanisms, consequences, and ethical implications of moving through different periods of time.");
    case ECoreScienceTech::Atomization:
        return TEXT("The process of breaking down a substance into its smallest particles or atoms, often used in physical chemistry and materials science to create fine powders or aerosols.");
    case ECoreScienceTech::ChronoBiology:
        return TEXT("The study of biological rhythms and the effects of time on living organisms, including circadian rhythms, seasonal behaviors, and the impact of time-related changes on health and development.");
    case ECoreScienceTech::Genetics:
        return TEXT("The study of heredity and the variation of inherited traits in organisms, focusing on the structure, function, and transmission of genes.");
    case ECoreScienceTech::Oceanography:
        return TEXT("The study of the Earth's oceans, including their physical properties, chemical composition, biological ecosystems, and the processes that govern ocean circulation and marine life.");
    case ECoreScienceTech::Ecology:
        return TEXT("The study of the interactions between living organisms and their environment, exploring how species coexist, compete, and evolve within ecosystems.");
    case ECoreScienceTech::EnvironmentalScience:
        return TEXT("The interdisciplinary study of the environment and the solutions to environmental problems, combining aspects of biology, chemistry, physics, and earth sciences to address issues like pollution, conservation, and climate change.");
    case ECoreScienceTech::ScientificEngineering:
        return TEXT("The application of scientific principles to the design, construction, and optimization of machines, structures, and systems, often involving problem-solving and innovation in various fields of engineering.");
    case ECoreScienceTech::ScientificTechnician:
        return TEXT("A professional who supports scientific research and development by maintaining equipment, conducting experiments, and analyzing data, often working in laboratories or industrial settings.");
    case ECoreScienceTech::LunarSciences:
        return TEXT("The study of the Moon, including its formation, geology, surface conditions, and the effects of its presence on Earth and the solar system.");
    case ECoreScienceTech::SolarSciences:
        return TEXT("The study of the Sun, its structure, energy production, and the effects of solar activity on the solar system, including phenomena like solar flares and space weather.");
    case ECoreScienceTech::SeasonalSciences:
        return TEXT("The study of seasonal changes and their impact on the environment, climate, and biological processes, including the examination of weather patterns, plant life cycles, and animal behaviors.");
    case ECoreScienceTech::CelestialSciences:
        return TEXT("The study of celestial energies, such as celestial entities, beings, energies, and spiritual energies, including their form, transformation, and the contracts that they can form and the bindings that are made by celestial powers.");
    case ECoreScienceTech::Energetics:
        return TEXT("The study of energy flow and transformation within physical, biological, and chemical systems, often focusing on the efficiency and sustainability of energy use in various processes.");
    case ECoreScienceTech::ParticlePhysics:
        return TEXT("The branch of physics that studies the fundamental particles of the universe and their interactions, often involving high-energy experiments to understand the building blocks of matter.");
    case ECoreScienceTech::Psychology:
        return TEXT("The scientific study of the mind and behavior, exploring how individuals think, feel, and act, as well as the underlying cognitive and emotional processes.");
    case ECoreScienceTech::PsychologyOfPlay:
        return TEXT("The study of the role of play in human development, focusing on how play influences learning, creativity, socialization, and psychological well-being.");
    case ECoreScienceTech::Inventions:
        return TEXT("The creation of new devices, processes, or technologies that solve problems or improve existing methods, often involving innovation, experimentation, and practical application.");
    case ECoreScienceTech::ScientificDesigns:
        return TEXT("The process of creating and planning new systems, structures, or technologies based on scientific principles, aimed at solving specific problems or advancing knowledge in a particular field.");
    case ECoreScienceTech::Meteorology:
        return TEXT("The study of the atmosphere and weather, focusing on the prediction and analysis of weather patterns, climate change, and atmospheric phenomena.");
    case ECoreScienceTech::Astrology:
        return TEXT("A belief system that suggests a connection between the positions of celestial bodies and events in human lives, often used for horoscopes and personal insight, though not considered a science.");
    case ECoreScienceTech::GameEngineDevelopment:
        return TEXT("The process of designing and building the software frameworks that power games, enabling the creation of graphics, physics, and interactive elements that define gameplay.");
    }
    return FString();
}

FString UCoreSciencesLibrary::DefinitionOf_Advanced(EAdvancedBioNeuro V)
{
    switch (V)
    {
    case EAdvancedBioNeuro::Neuroevolution:
        return TEXT("The study of the evolutionary processes that shape the development and function of the nervous system, often focusing on the genetic and environmental factors that influence brain structure and behavior.");
    case EAdvancedBioNeuro::Neuroscience:
        return TEXT("The scientific study of the nervous system, including the brain, spinal cord, and neural networks, aiming to understand how the brain controls behavior, processes information, and enables cognition.");
    case EAdvancedBioNeuro::Neuroanatomy:
        return TEXT("The study of the structure and organization of the nervous system, focusing on the anatomy of the brain and its connections with other parts of the body.");
    case EAdvancedBioNeuro::Biosynthesis:
        return TEXT("The process by which living organisms produce complex molecules from simpler ones, often involving the formation of proteins, nucleic acids, and other essential compounds.");
    case EAdvancedBioNeuro::Bioenergetics:
        return TEXT("The study of the flow and transformation of energy in living organisms, focusing on how cells produce and use energy for growth, metabolism, and other vital processes.");
    case EAdvancedBioNeuro::Biosynergetics:
        return TEXT("The study of the synergistic interactions within biological systems, exploring how different components work together to create complex, coordinated functions at the cellular or organismal level.");
    case EAdvancedBioNeuro::BiomechanicalFusion:
        return TEXT("The integration of biological and mechanical systems, often involving the development of prosthetics, implants, or robotic systems that interact seamlessly with living tissue.");
    case EAdvancedBioNeuro::Cybernetics:
        return TEXT("The interdisciplinary study of control and communication in animals, machines, and organizations, focusing on feedback systems, self-regulation, and adaptive behavior in both biological and artificial systems.");
    }
    return FString();
}

FString UCoreSciencesLibrary::DefinitionOf_EnergyPhys(EEnergyPhysicsEngineering V)
{
    switch (V)
    {
    case EEnergyPhysicsEngineering::FusionScience:
        return TEXT("The study of nuclear fusion, the process by which atomic nuclei combine to form a heavier nucleus, releasing energy, often explored as a potential source of clean, sustainable power.");
    case EEnergyPhysicsEngineering::NuclearScience:
        return TEXT("The study of atomic nuclei, their interactions, and the processes of nuclear fission and fusion, with applications in energy production, medicine, and industry.");
    case EEnergyPhysicsEngineering::FissionAndFusionScience:
        return TEXT("The combined study of nuclear fission, where atomic nuclei split, and fusion, where they combine, both processes that release vast amounts of energy, with applications in energy generation and weaponry.");
    case EEnergyPhysicsEngineering::Thermodynamics:
        return TEXT("The branch of physics that studies the relationships between heat, work, temperature, and energy, focusing on the laws that govern energy transfer and the behavior of systems in equilibrium.");
    case EEnergyPhysicsEngineering::Aerodynamics:
        return TEXT("The study of the behavior of air and other gases in motion, and the forces acting on objects as they move through these gases, with applications in aviation, automotive design, and sports.");
    case EEnergyPhysicsEngineering::ScienceOfFrequency:
        return TEXT("The study of how often events or cycles repeat over a given time period, often applied in fields like acoustics, electronics, and quantum mechanics, focusing on the properties and applications of various frequencies.");
    case EEnergyPhysicsEngineering::ScienceOfSequential:
        return TEXT("The study of ordered sequences and processes, often involving the arrangement of events, steps, or data in a specific order, with applications in computer science, mathematics, and biology.");
    case EEnergyPhysicsEngineering::ScienceOfProtons:
        return TEXT("The study of protons, the positively charged particles found in atomic nuclei, including their properties, behavior, and role in chemical reactions and nuclear physics.");
    case EEnergyPhysicsEngineering::ScienceOfElectrons:
        return TEXT("The study of electrons, the negatively charged particles that orbit atomic nuclei, focusing on their behavior, interactions, and role in electricity, magnetism, and chemical bonding.");
    case EEnergyPhysicsEngineering::ScienceOfGravity:
        return TEXT("The study of the force of gravity, which attracts objects with mass toward each other, exploring its effects on the motion of celestial bodies, the structure of the universe, and the behavior of objects on Earth.");
    case EEnergyPhysicsEngineering::ScienceOfAI:
        return TEXT("The study of creating machines and systems that can perform tasks requiring human-like intelligence, such as learning, reasoning, problem-solving, and understanding natural language.");
    case EEnergyPhysicsEngineering::ScienceOfGenes:
        return TEXT("The study of genes, the units of heredity in living organisms, focusing on their structure, function, and role in determining traits and guiding biological processes.");
    case EEnergyPhysicsEngineering::ScienceOfInheritance:
        return TEXT("The study of how traits, skills, abilities, and artistic talents are passed down from one generation to the next through genetic inheritance, epigenetics, and cultural transmission.");
    }
    return FString();
}

// Lists
TArray<ECoreScienceTech> UCoreSciencesLibrary::AllCore()
{
    return {
        ECoreScienceTech::QuantumScience,
        ECoreScienceTech::ComputerScience,
        ECoreScienceTech::Hardwares,
        ECoreScienceTech::Softwares,
        ECoreScienceTech::Bioevolution,
        ECoreScienceTech::Biological,
        ECoreScienceTech::Biochemistry,
        ECoreScienceTech::Geochemistry,
        ECoreScienceTech::Crystallography,
        ECoreScienceTech::Chrononautology,
        ECoreScienceTech::Atomization,
        ECoreScienceTech::ChronoBiology,
        ECoreScienceTech::Genetics,
        ECoreScienceTech::Oceanography,
        ECoreScienceTech::Ecology,
        ECoreScienceTech::EnvironmentalScience,
        ECoreScienceTech::ScientificEngineering,
        ECoreScienceTech::ScientificTechnician,
        ECoreScienceTech::LunarSciences,
        ECoreScienceTech::SolarSciences,
        ECoreScienceTech::SeasonalSciences,
        ECoreScienceTech::CelestialSciences,
        ECoreScienceTech::Energetics,
        ECoreScienceTech::ParticlePhysics,
        ECoreScienceTech::Psychology,
        ECoreScienceTech::PsychologyOfPlay,
        ECoreScienceTech::Inventions,
        ECoreScienceTech::ScientificDesigns,
        ECoreScienceTech::Meteorology,
        ECoreScienceTech::Astrology,
        ECoreScienceTech::GameEngineDevelopment
    };
}

TArray<EAdvancedBioNeuro> UCoreSciencesLibrary::AllAdvanced()
{
    return {
        EAdvancedBioNeuro::Neuroevolution,
        EAdvancedBioNeuro::Neuroscience,
        EAdvancedBioNeuro::Neuroanatomy,
        EAdvancedBioNeuro::Biosynthesis,
        EAdvancedBioNeuro::Bioenergetics,
        EAdvancedBioNeuro::Biosynergetics,
        EAdvancedBioNeuro::BiomechanicalFusion,
        EAdvancedBioNeuro::Cybernetics
    };
}

TArray<EEnergyPhysicsEngineering> UCoreSciencesLibrary::AllEnergyPhys()
{
    return {
        EEnergyPhysicsEngineering::FusionScience,
        EEnergyPhysicsEngineering::NuclearScience,
        EEnergyPhysicsEngineering::FissionAndFusionScience,
        EEnergyPhysicsEngineering::Thermodynamics,
        EEnergyPhysicsEngineering::Aerodynamics,
        EEnergyPhysicsEngineering::ScienceOfFrequency,
        EEnergyPhysicsEngineering::ScienceOfSequential,
        EEnergyPhysicsEngineering::ScienceOfProtons,
        EEnergyPhysicsEngineering::ScienceOfElectrons,
        EEnergyPhysicsEngineering::ScienceOfGravity,
        EEnergyPhysicsEngineering::ScienceOfAI,
        EEnergyPhysicsEngineering::ScienceOfGenes,
        EEnergyPhysicsEngineering::ScienceOfInheritance
    };
}

