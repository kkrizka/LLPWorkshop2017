#include <LLPWorkshop2017/Helpers.h>

#include <xAODTruth/TruthParticleContainer.h>

typedef ElementLink< xAOD::TruthParticleContainer > TruthParticleContainerLink;

bool isRHadron(const xAOD::TruthParticle& part) 
{
  if (part.status() != 1) return false;
  static const std::vector<int> pdgids = {1000993, 1009113, 1009213, 1009223, 1009313, 1009323, 1009333, 1091114, 1092114, 1092214, 1092224, 1093114, 1093214, 1093224, 1093314, 1093324, 1093334, 1000612, 1000622, 1000632, 1000642, 1000652, 1006113, 1006211, 1006213, 1006223, 1006311, 1006313, 1006321, 1006323, 1006333};
  if (std::find(pdgids.begin(), pdgids.end(), part.absPdgId()) == pdgids.end()) 
    return false;
  return true;
}
bool isChargedRHadron(const xAOD::TruthParticle& part) 
{
  if (part.status() != 1) return false;
  static const std::vector<int> pdgids = {1009213, 1009323, 1091114, 1092214, 1092224, 1093114, 1093224, 1093314, 1093334, 1000612, 1000632, 1000652, 1006211, 1006213, 1006223, 1006321, 1006323};
  if (std::find(pdgids.begin(), pdgids.end(), part.absPdgId()) == pdgids.end()) 
    return false;
  return true;
}

const xAOD::TruthParticle* getTruthParticle(const xAOD::IParticle* p)
{
  static const SG::AuxElement::ConstAccessor< TruthParticleContainerLink > a_truthParticleLink("truthParticleLink");

  if( !a_truthParticleLink.isAvailable( *p ) )
    return 0;
  const TruthParticleContainerLink& link = a_truthParticleLink(*p);
  if( !link.isValid() )
    return 0;

  return *link;
}
