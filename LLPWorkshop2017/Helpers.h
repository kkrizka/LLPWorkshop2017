#ifndef HELPERS_H_
#define HELPERS_H_

#include <xAODTruth/TruthParticle.h>

bool isRHadron(const xAOD::TruthParticle& part);
bool isChargedRHadron(const xAOD::TruthParticle& part);

const xAOD::TruthParticle* getTruthParticle(const xAOD::IParticle* p);

#endif // HELPERS_H_
