#include "verify.h"

bool Verify(const Domain *domain, const Problem *problem) {
    return VerifyDomain(domain) && VerifyProblem(domain, problem);
}
