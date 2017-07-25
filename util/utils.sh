
die() {
  # Print a message and exit with code 1.
  #
  # Usage: die <error_message>
  #   e.g., die "Something bad happened."

  echo -e $@
  exit 1
}

PLATFORM="$(uname -s | tr 'A-Z' 'a-z')"

function is_linux() {
  [[ "${PLATFORM}" == "linux" ]]
}

function is_macos() {
  [[ "${PLATFORM}" == "darwin" ]]
}

function is_windows() {
  # On windows, the shell script is actually running in msys
  [[ "${PLATFORM}" =~ msys_nt*|mingw*|cygwin*|uwin* ]]
}

function is_ppc64le() {
  [[ "$(uname -m)" == "ppc64le" ]]
}

function sed_in_place() {
  sed -e $1 $2 > "$2.bak"
  mv "$2.bak" $2
}

function write_to_bazelrc() {
  echo "$1" >> $BAZELRC
}

function write_action_env_to_bazelrc() {
  write_to_bazelrc "build --action_env $1=\"$2\""
}

function update_interactive_mode_status() {
  INTERACTIVE_MODE="1"

  if [[ ( $DEBIAN_FRONTEND == "noninteractive" ) || ( $DOCKER_FRONTEND == "noninteractive" ) ]]; then
    INTERACTIVE_MODE="0"
  fi

  local fd=0   # stdin, to test non-interactive

  if [[ ! -t "$fd" && ! -p /dev/stdin ]]; then
    INTERACTIVE_MODE="0"
  fi

}

[ -z $INTERACTIVE_MODE ] && update_interactive_mode_status

function is_non_interactive_mode() {
  [[ ${INTERACTIVE_MODE} == "0" ]]
}

function is_interactive_mode() {
  [[ ${INTERACTIVE_MODE} == "1" ]]
}



