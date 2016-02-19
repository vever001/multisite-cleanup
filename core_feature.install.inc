<?php

/**
 * @file
 * Install, update and uninstall helper functions.
 */

/**
 * Deletes some default content and modules from the platform we don't need.
 */
function _core_feature_cleanup_multisite() {
  // Remove page and editorial_team content.
  $results = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', array('page', 'editorial_team'))
    ->execute();
  foreach ($results as $result) {
    $nids[] = $result->nid;
  }
  if (!empty($nids)) {
    node_delete_multiple($nids);
  }

  // Disable and uninstall some modules we don't need.
  $modules_list = array(
    'color',
    'dashboard',
    'advanced_help',
    'multisite_autosave',
    'autosave',
    'css_injector',
    'email',
    'multisite_drupal_features_set_standard',
    'feature_set',
    'flag',
    'fpa',
    'help',
    'multisite_dynamic_basetheme',
    'simpletest',
    'context_layouts',
    'context_entity_field',
    'quicktabs',
    'js_injector',
    'views_bootstrap',
    'bootstrap_gallery',
    'tagclouds',
    'filefield_sources_plupload',
    'filefield_sources',
    'plupload',
    'og_linkchecker',
    'linkchecker_bean',
    'scheduler_workbench',
    'scheduler',
    'node_export',
    'multisite_settings_standard',
    'nexteuropa_editorial',
    'nexteuropa_europa_search',
    // UI modules.
    'field_ui',
    'context_ui',
    'rules_admin',
    'ds_ui',
    'coffee',
    'views_ui',
    'video_ui',
    // TMGMT / OG.
    'tmgmt_locale',
    'tmgmt_local',
    'tmgmt_language_combination',
    'tmgmt_file',
    'tmgmt_i18n_string',
    'tmgmt_og',
    'og_ui',
    'og_access',
    'og_context',
    'workbench_og',
    'og',
    'tmgmt_workbench',
    'tmgmt_entity_ui',
    'tmgmt_entity',
    'tmgmt_field',
    'tmgmt_ui',
    'tmgmt',
  );
  module_disable($modules_list, FALSE);
  drupal_uninstall_modules($modules_list, FALSE);

  // Cleanup editorial_team related stuff.
  node_type_delete('editorial_team');
  $instance = field_info_instance('comment', 'comment_body', 'comment_node_editorial_team');
  field_delete_instance($instance);
  field_purge_batch(100);
  user_role_delete('editorial team member');

  // Remove orphaned actions.
  actions_synchronize(actions_list(), TRUE);

  // Rebuild content permissions.
  node_access_rebuild();
}
