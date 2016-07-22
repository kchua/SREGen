# Sight Reading Exercise Generator
SREGen is a project I decided to undertake when I started to play the
piano more often again, and realized that I am a horrible sight reader.
Ultimately, I hope that SREGen would be able to successfully generate
sight reading exercises whose difficulty can be controlled, and at the same
time, sound musical by following the rules set by music theory.

## Usage (Command Line Arguments)

*Note: This program generates .ly files; compilation into .pdf files
requires an existing installation of Lilypond (installer can be
downloaded [here](http://lilypond.org)).*

### Scale Generator
Example 1 (generates an A&#9839; natural minor scale):

```
  SREGen -s ais -t nminor
```
Example 2 (generates a B&#9837; major scale):

```
  SREGen -t major -s bes
```

*The following arguments can be specified in any order.*

`-s <scale root>` (required) Specifies the root of the scale. The suffix
  -es represents a flat, and the suffix -is represents a sharp.

`-t major | nminor | hminor | mminor` (required) Specifies the tonality of
  the scale (whether the generated scale will be a major, natural minor,
  harmonic minor, or a melodic minor scale).

## Progress
Will add a few features to the note class before moving on to defining
chords and standard progressions.

## Tentative Project Plan
  1. Internal representation of notes and scales
  2. Writing voices onto a pdf file
  3. **Internal representation of other musical concepts (chords, progressions)**
  4. Creation of genetic algorithm template
  5. Harmony generation
  6. Bass line generation
  7. Two-part music
  8. Four-part music generation
  9. Ornamentation
  10. Construction of complex music from four-part music (stretch)
