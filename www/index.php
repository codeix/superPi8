<!DOCTYPE html>

<head>

   <!-- player skin -->
   <link rel="stylesheet" type="text/css" href="flowplayer/skin/minimalist.css">

   <!-- site specific styling -->
   <style type="text/css">
   body { font: 12px "Myriad Pro", "Lucida Grande", sans-serif; text-align: center; padding-top: 5%; }
   .flowplayer { width: 80%; }
   </style>

   <!-- flowplayer depends on jQuery 1.7.1+ (for now) -->
   <script type="text/javascript" src="jquery-2.0.3.min.js"></script>

   <!-- include flowplayer -->
   <script type="text/javascript" src="flowplayer/flowplayer.min.js"></script>
   
   <!-- project stuff -->
   <link rel="stylesheet" type="text/css" href="8mmovie.css">
   <script type="text/javascript" src="8mmovie.js"></script>


</head>

<body>

    <?php

        $DIR=realpath('../movies/movies');


        $page = 'project';
        if (in_array($_REQUEST['page'], array('list', 'snapshot', 'player')))
            $page = $_REQUEST['page'];
    ?>

    <header>

        <nav>
            <ul>
                <li class="<?php echo $page == 'project'?'selected':''; ?>"><a href="?page=project">The Project</a></li>
                <li class="<?php echo $page == 'list'?'selected':''; ?>"><a href="?page=list">Movie List</a></li>
                <li class="<?php echo $page == 'snapshot'?'selected':''; ?>"><a href="?page=snapshot">Last Snapshot</a></li>
                <?php if($page == 'player') { ?>
                    <li class="<?php echo $page == 'player'?'selected':''; ?>"><a href="?page=list">Player</a></li>
                <?php } ?>
            </ul>
        </nav>
    </header>

    <?php if($page == 'project'){ ?>
    <section id="project" class="content">
        <img src="scanner1.jpg" />
        <img src="scanner2.jpg" />
        <img src="scanner3.jpg" />
    </section>
    <?php } ?>

    <?php if($page == 'list'){ ?>
    <section id="list" class="content">

        <?php
            $entries = array();
            foreach(glob($DIR.'/*', GLOB_ONLYDIR) as $entry) {
                $name = basename($entry);
                $base = $entry.'/'.$name;
                $file = $base.'.mp4';
                if (!file_exists($base.'.webm') || !file_exists($base.'.web.mp4'))
                    continue;
                $entries[] = array('file' => $file, 'name' => $name);
            }
            usort($entries, function ($a, $b) {
                return filemtime($b['file']) - filemtime($a['file']);
            });
        ?>

       <h1><?php echo sizeof($entries);?> movies in total</h1>

        <table>
            <thead>
                <tr>
                    <td>Scan date</td>
                    <td>Name</td>
                    <td>Download</td>
                    <td>Play</td>
                </tr>
            </thead>
            <tbody>
            <?php

                foreach($entries as $entry){
                   $file = $entry['file'];
                   $name = $entry['name'];
            ?>
                    <tr>
                        <td><?php echo date("d.m.Y H:i:s",filemtime($file)) ?></td>
                        <td><?php echo $name ?></td>
                        <td>
                            <a href="movie.php?type=mp4&download=true&movie=<?php echo base64_encode($name) ?>">mp4</a>
                            <a href="movie.php?type=webm&download=true&movie=<?php echo base64_encode($name) ?>">webm</a>
                        </td>
                        <td><a href="?page=player&movie=<?php echo base64_encode($name) ?>">Play</a></td>
                    </tr>
           <?php } ?>
           </tbody>
        </table>
    </section>
    <?php } ?>

    <?php if($page == 'snapshot'){ ?>
    <section id="snapshot" class="content">
        <img src="snapshot.php"/>
    </section>
    <?php } ?>

    <?php if($page == 'player'){ ?>
    <section id="player" class="content">
        

        <h2><?php echo base64_decode($_REQUEST['movie']); ?><h2/>
        <!-- the player -->
        <div class="flowplayer" data-native_fullscreen="true" data-swf="flowplayer/flowplayer.swf"  data-ratio="0.75">
            <video autoplay>
                <source type="video/webm" src="movie.php?type=webm&movie=<?php echo $_REQUEST['movie']?>">
                <source type="video/mp4" src="movie.php?type=mp4&movie=<?php echo $_REQUEST['movie']?>">
            </video>
        </div>
    </section>
    <?php } ?>

    <footer>
      <p>by samuel riolo 2013</p>
    </footer>

</body>
