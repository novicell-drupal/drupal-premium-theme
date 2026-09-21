# Drupal Premium Theme
Base theme for our premium sites.
So be careful when making changes/fixing bugs, this theme can be updated on already built sites.

# JavaScript and CSS (PostCSS) for development.
Run ```npm ci``` to install.

Commands to build to dist folder:
```
npm run build:css
npm run build:webpack
```


## Gin Layout Builder compatibility

Gin is optional. The integration activates only when Gin LB and Gin Toolbar are
both enabled and Composer 2 reports this supported package combination:

- Gin: `>=5.0.0 <6.0.0-dev`
- Gin Toolbar: `>=3.0.0 <4.0.0-dev`
- Gin LB: `>=3.0.0-beta1 <4.0.0-dev`

For that combination, forms marked `#gin_lb_form` keep Gin LB template
suggestions and input/textarea classes. Unmarked frontend forms retain Premium
markup. Only the four legacy editor CSS libraries (`dialog`, `layout-builder`,
`layout-builder-form`, `layout-builder-form-confirmation`) are emptied; their
names remain valid for older template attachments. Other Premium libraries,
including status messages and lazy loading, are unchanged. Core/Claro dialog
styles remain available.

Disabled/missing modules, older or future major package versions, missing
Composer metadata and development branches keep the legacy behavior, including
Premium form suggestions/classes and editor CSS. In that fallback, the old
Claro-dialog and off-canvas-reset overrides are applied conditionally through
`hook_library_info_alter()` instead of unconditionally in the theme info file.
Branch aliases do not opt untested development versions into the integration.
Rebuild Drupal caches after updating packages or changing enabled modules.

Shared toolbar, section-header and Gin/Styles refinements belong in the optional
`novicell/premium_gin` module. This frontend theme does not force installation of
Gin; Premium Gin declares its requirements through Composer.

Run `php tests/gin-lb-compatibility.php` for supported/unsupported version,
module enablement, form, dialog and library checks. Also run
`php tests/gin-lb-compatibility.php --without-composer` to check the fallback
when Composer's installed-version API is unavailable. Tests are standalone and
do not modify an installed site's Composer metadata or module configuration.
Feature branch base: `2.x` (the repository has no `main` branch).
