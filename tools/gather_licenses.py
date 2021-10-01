import sys
import argparse
import urllib.request
import ssl
import certifi


class RET:
    OK = 0
    FAIL = 1


def get_text_from_url(url):
    context = ssl.create_default_context(cafile=certifi.where())
    response = urllib.request.urlopen(url, context=context)
    data = response.read()

    return data.decode("utf-8")


def make_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument("-o", "--output-file", type=str, required=True,
        help="Output file with legal notices.")

    return parser.parse_args()


def gather_licenses(output_file):
    ret = RET.OK
    texts = {}
    files = {
        "glew":     "https://raw.githubusercontent.com/nigels-com/glew/master/LICENSE.txt",
        "glfw":     "https://raw.githubusercontent.com/glfw/glfw/master/LICENSE.md",
        "assimp":   "https://raw.githubusercontent.com/assimp/assimp/master/LICENSE",
        "freetype": "https://gitlab.freedesktop.org/freetype/freetype/-/raw/master/LICENSE.TXT",
        "glm":      "https://raw.githubusercontent.com/g-truc/glm/master/copying.txt",
        "pfd":      "https://raw.githubusercontent.com/samhocevar/portable-file-dialogs/master/COPYING",
        "spdlog":   "https://raw.githubusercontent.com/gabime/spdlog/v1.x/LICENSE",
        "stb":      "https://raw.githubusercontent.com/nothings/stb/master/LICENSE"
    }

    # Fill the dictionary with the license texts. We avoid directly writing
    # to file, in case an error occurs, e.g. a URL has changed.
    for k, v in files.items():
        try:
            texts[k] = get_text_from_url(v)

        except Exception as e:
            ret = RET.FAIL
            print("Cannot gather licenses: ", str(e))

    # Write the license texts to the legal notices file
    if ret == RET.OK:
        try:
            with open(output_file, "w") as fo:
                header = "\n".join(["=" * 80, "THIRD PARTY LICENSES", "=" * 80, "", ""])
                fo.write(header)

                for k, v in texts.items():
                    header = "\n".join(["", "", "-" * 80, k, "-" * 80, "", "", "", ""])
                    fo.write(header)

                    contents = "\n".join([v, "", ""])
                    fo.write(contents)

        except Exception as e:
            ret = RET.FAIL
            print("Cannot write to output file: ", str(e))

    if ret == RET.OK:
        print("Done")

    return ret


if __name__ == "__main__":
    args = make_parser()
    ret = gather_licenses(args.output_file)
    sys.exit(ret)
