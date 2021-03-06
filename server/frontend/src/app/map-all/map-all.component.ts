import { Component, OnInit } from '@angular/core';


import 'ol/ol.css';
import * as olsource from 'ol/source';
import Map from 'ol/Map';
import {Tile as TileLayer} from 'ol/layer';
import Overlay from 'ol/Overlay';
import View from 'ol/View';
import { fromLonLat, toLonLat } from 'ol/proj';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { Vector } from 'ol/layer';
import OSM from 'ol/source/OSM';
import Style from 'ol/style/Style';
import Icon from 'ol/style/Icon';
import VectorSource from 'ol/source/Vector';

import { DeviceService } from '../service/device.service';
import { Device } from '../device/device.component';
import { ActivatedRoute } from '@angular/router';
import {toStringHDMS} from 'ol/coordinate';
import { SweepService } from '../service/sweep.service';
import { Sweep } from '../sweep/sweep.component';
import { Subscription } from 'rxjs';


@Component({
  selector: 'map-all',
  templateUrl: './map-all.component.html',
  styleUrls: ['./map-all.component.css']
})
export class MapAllComponent implements OnInit {
  subscription1: Subscription;
  subscription2: Subscription;
  markers: Feature[] = [];
  constructor(private service: DeviceService,  private route : ActivatedRoute, private sweepService: SweepService) { 
    this.subscription1 = this.service.allDeviceInfo().subscribe(x => { console.log("From interval 1000",x);
    this.devices = x;
  });
    this.subscription2 = this.sweepService.getLatestSweep().subscribe(x => { console.log("hi ",x);
    this.latestSweep = x;
    this.devices.forEach((device: Device) => {
      if (device.latitude != null && device.longitude != null){
        let device_location = fromLonLat([device.longitude, device.latitude]);
         
        
        var aFeature = new Feature({
          geometry: new Point(device_location)
        })
        var aFeatureStyle = new Style({
          image: new Icon({
            src: '../../assets/images/star.png',
          }),
        })
  
        aFeature.setStyle(aFeatureStyle);
        //console.log("print ",this.service.getDeviceDescription(device,this.latestSweep));
        let temp: number|any = this.sweepService.getLatestTemp(device,this.latestSweep);
        let strength: number|any = this.sweepService.getLatestStrength(device,this.latestSweep);
        aFeature.set('description',this.service.getDeviceDescription(device,temp,strength));
        
        this.markers.push(aFeature);
      }
    })
    console.log("markers ",this.markers);
    this.map = new Map({
      target: 'all_device_map',
      layers: [
        new TileLayer({
          source: new OSM()
        })
      ],
      view: new View({
        center: fromLonLat([this.longitude,this.latitude]),
        zoom: this.zoom
      })
    });
        let layer = new Vector({
          source: new VectorSource({features: this.markers})  
        })
       
        this.map.addLayer(layer);

        let container: HTMLElement | null = document.getElementById('popup');
        let content = document.getElementById('popup-content');
        let closer = document.getElementById('popup-closer');

        

        var popup = new Overlay({
          element: <HTMLInputElement> container,
          autoPan: true,
          autoPanAnimation: {
          duration: 250,
          },
          stopEvent: false,
        });

        this.map.addOverlay(popup);

        // display popup on click
        this.map.on('click',  (evt) => {
          const feature = this.map.forEachFeatureAtPixel(evt.pixel, function (feature: any) {
            return feature;
          });
          if (feature) {
            const coordinate = evt.coordinate;
            const hdms = toStringHDMS(toLonLat(coordinate));
        
            container.innerHTML = feature.get('description');
            popup.setPosition(coordinate);
          } else {
            popup.setPosition(undefined)
          }
        });
  });
 

  }

  ngOnDestroy() {
    this.subscription1.unsubscribe()
    this.subscription2.unsubscribe()
}

  map!: Map;
  zoom: number = 12;
  latitude: number = 41.079990;
  longitude: number = -85.138601;
  $: any;
  latestSweep: Sweep[] = [];
  devices: Device[] = [];

  

  ngOnInit(): void {
    
   }

}