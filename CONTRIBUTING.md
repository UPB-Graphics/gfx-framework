# How to contribute


## General

-   The official contact for project development is Github, which is used for
    code review, issues, suggestions, and long term discussions.
-   By making a contribution to this project, you agree to
    [Developer's Certificate of Origin 1.1](https://developercertificate.org/).


## Sending patches

-   For small changes, send a pull request through GitHub
-   For bigger changes, talk to us first before you start working on them.
    It will make code review easier for both parties later on.
-   Be sure to ***test your changes*** on as many platforms as possible. If
    you didn't, or you only did on some platforms, make sure to say so in the
    commit message and the pull request text.


## Write good commit messages

-   Write informative commit messages. Use present tense to describe the
    situation with the patch applied, and past tense for the situation before
    the change.
-   A summary of good conventions: https://chris.beams.io/posts/git-commit/


## Split changes into multiple commits

-   Follow git good practices, and split independent changes into several
    commits. It's usually okay to put them into a single pull request.
-   Try to separate cosmetic and functional changes. It's okay to make a few
    additional cosmetic changes in the same file you're working on, however,
    avoid doing something like reformatting a whole file, and hiding an actual
    functional change in the same commit.
-   Splitting changes does ***not*** mean that you should make them as fine-grained
    as possible. Commits should form logical steps in development. The way you
    split changes is important for code review and analyzing bugs.
