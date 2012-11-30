<?php
/**
 * @file
 * The theme implementation file.
 *
 * Contains the theme hook for the content admin page and the Chartbeat API
 * implementation.
 */

/**
 * Theme registry callback for the content administration page.
 */
function theme_chartbeat_node_admin_nodes($form) {
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $has_posts = isset($form['title']) && is_array($form['title']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';

  $concurrent = $has_posts ? _chartbeat_concurrent_api_call($form['title']) : '';
  $header = array(
    $select_header,
    t('Title'),
    t('Type'),
    t('Author'),
    t('Status'),
  );
  if (isset($form['language'])) {
    $header[] = t('Language');
  }
  $header[] = t('Operations');
  $output = '';
  $header[] = t('Concurrent Views');

  $output .= drupal_render($form['options']);
  if ($has_posts) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['nodes'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['status'][$key]);
      if (isset($form['language'])) {
        $row[] = drupal_render($form['language'][$key]);
      }
      $row[] = drupal_render($form['operations'][$key]);
      $row[] = $concurrent[$key];
      $rows[] = $row;
    }

  }
  else {
    $rows[] = array(array(
        'data' => t('No posts available.'),
        'colspan' => '7',
      ));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * Helper function for calling the Chartbeat API.
 *
 * @param array $nid_array
 *   Array of node attributes as produced by the content administration form.
 *
 * @return array
 *   An array of ints representing concurrent visitors, keyed to the node which
 *   the visitors are on.
 */
function _chartbeat_concurrent_api_call($nid_array) {
  $api = variable_get('chartbeat_api_key', FALSE);

  $ret_array = array();

  foreach (element_children($nid_array) as $node) {
    $node_arr = $nid_array[$node];
    preg_match("`/node/[\d]*`", $node_arr['#value'], $matches);
    $href = $matches[0];
    if ($api) {
      $host = $_SERVER['SERVER_NAME'];
      $req_http = "http://api.chartbeat.com/live/summary/v3/?apikey=$api&host=$host&keys=read&path=$href";
      $resp = drupal_http_request($req_http);
      $resp_code = (int) $resp->code;
      if ($resp_code == 200) {
        $response = json_decode($resp->data, TRUE);
        if (!empty($response)) {
          $num = (int) $response['read']['data']['sum'];
          $ret_array[$node] = $num;
        }
        else {
          $ret_array[$node] = 0;
        }
      }
      else {
        $ret_array[$node] = 'n/a';
      }
    }
    else {
      $ret_array[$node] = 'n/a';
    }
  }

  return $ret_array;
}
