# Second Movement: Catholic Liturgical Edition

This is a customized fork of the [Movement](https://github.com/joeycastillo/Sensor-Watch) firmware for [Sensor Watch](https://www.sensorwatch.net), specifically tailored for the Catholic Liturgical Calendar.

## Overview

This edition of Movement is designed to help the wearer live the Liturgical Year. It repurposes the standard watch indicators to provide at-a-glance information about the current day's liturgical and civil significance, and includes dedicated faces for browsing the calendar. Other Movement faces and configs have also been lightly modified. 

The Catholic Liturgical information has been gathered from [gCatholic](https://gcatholic.org/calendar/) and from Fr. John Romanodorazio's [Litcal](https://litcal.johnromanodorazio.com/).

*Note: The liturgical calendar information is pre-loaded. The normal calendar goes unitl 2035-12-01 and the traditional until 2029-12-31.*

### The Indicator System
In this edition, the standard LCD indicators are used to signal the status of "Today" and "Tomorrow":

| Indicator | Meaning |
| :--- | :--- |
| **Bell** (Alarm) | **Feast Day / Special Liturgy**: Today is a significant feast or special liturgical day. | 
| **LAP** | **Fast / Abstinence**: Today is a day of prescribed fasting or abstinence (e.g., Lent, Fridays), or the watch's battery is low. | 
| **Signal** | **Special Day**: Today is a civil holiday or special anniversary (e.g., New Year, Mother's Day). | 
| **24H** | **Vigil / Anticipation**: Tomorrow is either a Liturgical Feast or a Special Day. | 

---

## Included Watch Faces

### 1. Clock Face
The primary clock face displays the time and date. It respects the global indicator system described above, allowing you to see the day's status without leaving the main screen.

### 2. Liturgical Calendar Face
Displays the current Liturgical Season and the specific feast or saint of the day.
- **Top Row**: Displays the current Season (e.g., `AD` for Advent, `LE` for Lent, `OT` for Ordinary Time) and the week of that season.
- **Bottom Row**: Displays the name of the feast or saint. If the text is longer than 6 characters, it will loop continuously.
- **Interactions**:
    - **Alarm Button**: Press to scroll forward through future feast days.
    - **Mode Long Press**: Toggles between the **Normal** (Modern) and **Traditional** (1962) calendars. The watch will briefly display "Normal" or "Trad" at the bottom to confirm the change.

### 3. Special Days Face
Displays civil holidays and common special days. Special days are configured face is configured in watch-faces/clock/special_days.h
- **Top Row**: Displays the date numerically (Month on the left, Day on the right).
- **Bottom Row**: Displays the holiday name (e.g., "New Year", "Mothers Day"). Long text loops continuously.
- **Interactions**:
    - **Alarm Button**: Press to jump to the next holiday in the list.
- **Calculated Holidays**: This face dynamically calculates the "floating" holidays of Mother's Day (2nd Sunday of May) and Father's Day (3rd Sunday of June) based on the current year.

### 4. Support Faces
Standard Movement faces are included for utility:
- **Moon Phase**: Shows the current phase of the moon. Edited to show percent fullness. 
- **Sunrise/Sunset**: Shows solar times based on your location.
- **Fast Stopwatch**: A high-precision timer.
- **Tally**: A simple counter.

---

## Configuration

### Setting your Location
For the **Sunrise/Sunset** face to be accurate, you should ensure your latitude and longitude are set. This can be done in the simulator or via the `Sunrise/Sunset` settings page on the watch.

### Modifying Special Days
The list of civil holidays is stored in `watch-faces/clock/special_days.h`. You can add your own anniversaries or local holidays to this list by following the `SpecialDayDefinition` structure.

---

## Getting Started

### Dependencies
You will need the **GNU Arm Embedded Toolchain**. On Debian/Ubuntu: `apt install gcc-arm-none-eabi`.

### Installation
1.  **Fetch Submodules**:
    ```bash
    git submodule update --init --recursive
    ```
2.  **Activate Emscripten** (if using the simulator):
    ```bash
    source ../emsdk/emsdk_env.sh
    ```
3.  **Build for Hardware**:
    ```bash
    make BOARD=sensorwatch_red DISPLAY=classic
    ```
    *(Replace `sensorwatch_red` with your board type: `pro`, `green`, `blue`, or `red`)*.

4.  **Install**:
    Double-tap Reset on the watch to enter `WATCHBOOT` mode, then:
    ```bash
    make install
    ```

### Using the Simulator
To test changes in your browser:
```bash
emmake make BOARD=sensorwatch_red DISPLAY=classic
python3 -m http.server -d build-sim
```
Then visit [http://localhost:8000/firmware.html](http://localhost:8000/firmware.html).