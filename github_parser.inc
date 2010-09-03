<?php
/**
* Parses GitHub Feeds
*/
class GitHubParser extends FeedsParser {

  /**
  * Parses a raw string and populates FeedsImportBatch object from it.
  */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    // Get the file's content.
    $string = $batch->getRaw();
    
    // Parse it...
    $xml = simplexml_load_string($string);
    $entries = array();
    
    foreach($xml->entry as $v){

      $link = (array)$v->link;
      $raw_link = $link['@attributes']['href'];

      // Removing the extra title that GitHub feeds leave in
      preg_match_all('$<[^>]+>(.*)</[^>]+>$Us',
          (string)$v->content,
          $split_content, PREG_PATTERN_ORDER);
       
      $entries[] = array(
        'guid' => (string)$v->id,
        'title' => (string)$v->title,
        'body' => $split_content[0][0],
        'url' => $raw_link,
        'post_date' => strtotime((string)$v->updated)
      );
        
    }


    // Populate the FeedsImportBatch object with the parsed results.
    $batch->setTitle('GitHub Batch Feeds'); 
    $batch->setItems($entries);
  }

  public function getMappingSources() {
    return array(
      'guid' => array(
        'name' => t('GUID'),
        'description' => t('Unique ID.'),
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('Feed Title.'),
      ),
      'body' => array(
        'name' => t('Body'),
        'description' => t('Feed Body.'),
      ),
      'url' => array(
        'name' => t('URL'),
        'description' => t('Feed url.'),
      ),
      'post_date' => array(
        'name' => t('Post Date'),
        'description' => t('The date/time this feed item posted.'),
      ),
    );
  }

}