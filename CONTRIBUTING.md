# How to contribute


## General

-   The official channel of contact is Github, which is used for code review,
issues, suggestions, and long term discussions.


## Sending patches

-   Make a Github pull request.
-   Never send patches to any of the developers email addresses.
-   Be sure to ***test your changes*** on as many platforms as possible. If
    you didn't, or you did only on some platforms, please say so in the commit
    message and the pull request text.


## Write good commit messages

-   Write informative commit messages. Use present tense to describe the
    situation with the patch applied, and past tense for the situation before
    the change.
-   Another summary of good conventions: https://chris.beams.io/posts/git-commit/


## Split changes into multiple commits

-   Follow git good practices, and split independent changes into several
    commits. It's usually okay to put them into a single pull request.
-   Try to separate cosmetic and functional changes. It's okay to make a few
    additional cosmetic changes in the same file you're working on, however,
    avoid doing something like reformatting a whole file, and hiding an actual
    functional change in the same commit.
-   Splitting changes does *not* mean that you should make them as fine-grained
    as possible. Commits should form logical steps in development. The way you
    split changes is important for code review and analyzing bugs.


## Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

-   (a) The contribution was created in whole or in part by me and I
    have the right to submit it under the open source license
    indicated in the file; or

-   (b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the same open source license (unless I am
    permitted to submit under a different license), as indicated
    in the file; or

-   (c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

-   (d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it, including my sign-off) is
    maintained indefinitely and may be redistributed consistent with
    this project or the open source license(s) involved.
