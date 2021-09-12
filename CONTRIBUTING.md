# How to contribute


## General

-   The official contact method for project development is Github, which is used for
    code review, issues, suggestions, and long term discussions.
-   By making a contribution to this project, you agree to
    [Developer's Certificate of Origin 1.1](https://developercertificate.org/).


## Before you start

-   For small changes, you can send a pull request directly through Github.
-   For bigger changes, talk to us first before you start working on them.
    It will make code review easier for both parties later on.


## During development

-   Fork this repo and make your own.
-   Make one dedicated branch per feature; if you want to do more than one thing,
    make more than one branch. We will not merge from your `master` branch.
-   Write informative commit messages. See https://chris.beams.io/posts/git-commit/
-   ***Split an independent change into its own commit.*** This does not mean that
    you should make commits as fine-grained as possible. Commits should form
    logical steps in development. The way you split changes is important
    for code review and analyzing bugs.
-   ***Separate cosmetic and functional changes.*** It's okay to make a few
    additional cosmetic changes in the same file you're working on, however,
    avoid doing something like reformatting a whole file ***and*** implementing an actual
    functional change in the same commit.


## After you finish

-   ***Test your changes*** on as many platforms as possible. If
    you didn't, or you only did on some platforms, make sure to say so in the
    commit message and the pull request text.
-   ***Document any change in behaviour.*** Make sure the `README.md` and any other
    relevant items of documentation are kept up-to-date.
