# Contributing

Contributions are welcome. By contributing, you dedicate your work under the same terms as the rest of this repository:

- Documentation, text, and original diagrams: [CC0-1.0](LICENSE-CC0).
- Code and scripts: [the Unlicense](UNLICENSE).

The file split is in [LICENSE](LICENSE). Third-party product images, logos, and screenshots are not covered. Do not add them unless you have rights that allow it. AI-generated images carry no ownership guarantees, and they are not wiring references.

## Add a project

1. Copy [`src/content/docs/projects/_template.md`](src/content/docs/projects/_template.md) to a new file under `src/content/docs/projects/`. The leading underscore keeps the blank file off the site.
2. Remove the underscore from the filename so Starlight will publish the page.
3. Fill every section. If you do not know a price, link, or spec, write `TBD` or `not checked`. Do not guess.
4. Put warnings and safety notes in Starlight asides (`:::note`, `:::tip`, `:::caution`, `:::danger`). Do not use GitHub alert syntax on the site.
5. Use **verified-compatible** only after a bench check you can describe. Until then the row is **illustrative**.
6. Open a pull request. The build check runs `npm run build`.

The same section list is explained on the site's [project template](https://alexmilligan.github.io/hardware-project-ideas/projects/template/) page.

## Scripts

A helper script checks its prerequisites before it changes anything. It never flashes or overwrites a board unless the person running it takes an explicit action. Each script names its tested or untested status, the OS, and the tool versions it was actually run with. If a version was not recorded, say so. Every script needs a manual fallback that does not require the script.

## Build locally

Node is pinned to 22.23.4 (see [`.node-version`](.node-version)). The write-ups live in `src/content/docs/`. From a checkout:

```sh
npm ci
npm run dev
npm run build
```

`npm run dev` previews the site on your machine. `npm run build` writes the static site into `docs/`, which is the folder GitHub Pages publishes from this repository. Commit that folder with the content change.
