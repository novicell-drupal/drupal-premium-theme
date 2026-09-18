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

When Gin LB marks a form with `#gin_lb_form`, Premium Theme leaves template
suggestions and input/textarea classes to Gin LB. Frontend forms retain Premium
markup. When Gin LB is enabled, only the four legacy editor CSS libraries
(`dialog`, `layout-builder`, `layout-builder-form`,
`layout-builder-form-confirmation`) are emptied; their names remain available
for older template attachments. Status messages and lazy loading are unchanged.
Without Gin LB, those legacy editor libraries remain available.

The global overrides that removed Claro's dialog stylesheet and core's
off-canvas reset have been removed, so Drupal/Gin can own their dialog styling.

Shared toolbar, section-header and Gin/Styles refinements belong in the optional
`novicell/premium_gin` module. This frontend theme does not require Gin 5;
Premium Gin declares that requirement through Composer.

Run `php tests/gin-lb-compatibility.php` for form/library regression checks.
Feature branch base: `2.x` (the repository has no `main` branch).
