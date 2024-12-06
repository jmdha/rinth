#pragma once

#include "domain.h"
#include "problem.h"

bool VerifyDomain(const Domain *domain);
bool VerifyProblem(const Domain *domain, const Problem *problem);
bool Verify(const Domain *domain, const Problem *problem);
