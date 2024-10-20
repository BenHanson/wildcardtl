# wildcardtl
wildcardtl is a header-only library for matching wildcards. The library requires a minumum of C++11 support.

## Example Usage
    #include <wildcardtl/wildcard.hpp>

    int main()
    {
        try
        {
            wildcardtl::wildcard wc;

            // Second parameter is icase
            wc.assign("[Cc]*ona", false);

            const bool match = wc.match("corona");

            std::cout << "Wildcard ";

            if (match)
                std::cout << "matched";
            else
                std::cout << "did not match";

            std::cout << '\n';
        }
        catch
        {
            std::cout << e.what() << '\n';
        }

        return 0;
    }

Note that "zero or more" defaults to '*', "any character" defaults to '?' and "not" defaults to '!'.
All of these can be overridden if required.
