#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Użycie: $0 <czas_w_sekundach>"
  exit 1
fi

TIME="$1"

if ! [[ "$TIME" =~ ^[0-9]+$ ]]; then
  echo "Błąd: czas musi być liczbą."
  exit 1
fi

PID=""

handle_interrupt() {
  echo
  echo "Przerwano przez Ctrl+C"

  if [ -n "$PID" ] && kill -0 "$PID" 2>/dev/null; then
    echo "Zatrzymuję proces (PID=$PID)"
    kill "$PID" 2>/dev/null
    wait "$PID" 2>/dev/null
  fi

  exit 2
}

trap handle_interrupt SIGINT

echo "Start zadania (sleep $TIME)..."
sleep "$TIME" &
PID=$!

echo "Proces działa w tle, PID=$PID"

wait "$PID"
STATUS=$?

if [ "$STATUS" -eq 0 ]; then
  echo "Zadanie zakończone poprawnie."
  exit 0
else
  echo "Zadanie zakończone błędem."
  exit 1
fi
